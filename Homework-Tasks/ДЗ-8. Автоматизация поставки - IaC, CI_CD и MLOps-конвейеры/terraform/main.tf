provider "aws" {
  region = var.aws_region

  default_tags {
    tags = merge(
      {
        project     = var.project_name
        environment = var.environment
      },
      var.tags
    )
  }
}

data "aws_availability_zones" "available" {
  state = "available"
}

locals {
  name_prefix = "${var.project_name}-${var.environment}"
  azs         = slice(data.aws_availability_zones.available.names, 0, 3)
}

module "vpc" {
  source  = "terraform-aws-modules/vpc/aws"
  version = "5.8.1"

  name = local.name_prefix
  cidr = var.vpc_cidr
  azs  = local.azs

  private_subnets = var.private_subnet_cidrs
  public_subnets  = var.public_subnet_cidrs

  enable_nat_gateway   = true
  single_nat_gateway   = true
  enable_dns_hostnames = true
  enable_dns_support   = true

  public_subnet_tags = {
    "kubernetes.io/role/elb" = "1"
  }

  private_subnet_tags = {
    "kubernetes.io/role/internal-elb" = "1"
    "karpenter.sh/discovery"          = local.name_prefix
  }
}

module "eks" {
  source  = "terraform-aws-modules/eks/aws"
  version = "20.13.1"

  cluster_name    = local.name_prefix
  cluster_version = var.eks_cluster_version

  vpc_id     = module.vpc.vpc_id
  subnet_ids = module.vpc.private_subnets

  cluster_endpoint_public_access           = true
  enable_cluster_creator_admin_permissions = true

  cluster_addons = {
    coredns    = {}
    kube-proxy = {}
    vpc-cni    = {}
  }

  eks_managed_node_groups = {
    general = {
      desired_size   = var.node_desired_size
      min_size       = var.node_min_size
      max_size       = var.node_max_size
      instance_types = var.node_instance_types
      capacity_type  = "ON_DEMAND"

      labels = {
        workload = "general"
      }
    }

    ml = {
      desired_size   = 1
      min_size       = 0
      max_size       = 3
      instance_types = ["m5.xlarge"]
      capacity_type  = "ON_DEMAND"

      labels = {
        workload = "ml-training"
      }

      taints = {
        ml = {
          key    = "dedicated"
          value  = "ml"
          effect = "NO_SCHEDULE"
        }
      }
    }
  }

  node_security_group_tags = {
    "karpenter.sh/discovery" = local.name_prefix
  }
}

resource "aws_kms_key" "artifacts" {
  description             = "KMS key for AI platform buckets and registries"
  deletion_window_in_days = 7
  enable_key_rotation     = true
}

resource "aws_ecr_repository" "app" {
  name                 = "${local.name_prefix}-${var.image_repository_name}"
  image_tag_mutability = "IMMUTABLE"

  image_scanning_configuration {
    scan_on_push = true
  }

  encryption_configuration {
    encryption_type = "KMS"
    kms_key         = aws_kms_key.artifacts.arn
  }
}

resource "aws_s3_bucket" "datasets" {
  bucket = var.dataset_bucket_name
}

resource "aws_s3_bucket_versioning" "datasets" {
  bucket = aws_s3_bucket.datasets.id

  versioning_configuration {
    status = "Enabled"
  }
}

resource "aws_s3_bucket_server_side_encryption_configuration" "datasets" {
  bucket = aws_s3_bucket.datasets.id

  rule {
    apply_server_side_encryption_by_default {
      kms_master_key_id = aws_kms_key.artifacts.arn
      sse_algorithm     = "aws:kms"
    }
  }
}

resource "aws_s3_bucket" "features" {
  bucket = var.feature_bucket_name
}

resource "aws_s3_bucket_versioning" "features" {
  bucket = aws_s3_bucket.features.id

  versioning_configuration {
    status = "Enabled"
  }
}

resource "aws_s3_bucket_server_side_encryption_configuration" "features" {
  bucket = aws_s3_bucket.features.id

  rule {
    apply_server_side_encryption_by_default {
      kms_master_key_id = aws_kms_key.artifacts.arn
      sse_algorithm     = "aws:kms"
    }
  }
}

resource "aws_s3_bucket" "model_registry" {
  bucket = var.model_registry_bucket_name
}

resource "aws_s3_bucket_versioning" "model_registry" {
  bucket = aws_s3_bucket.model_registry.id

  versioning_configuration {
    status = "Enabled"
  }
}

resource "aws_s3_bucket_server_side_encryption_configuration" "model_registry" {
  bucket = aws_s3_bucket.model_registry.id

  rule {
    apply_server_side_encryption_by_default {
      kms_master_key_id = aws_kms_key.artifacts.arn
      sse_algorithm     = "aws:kms"
    }
  }
}

resource "aws_s3_bucket" "vector_index" {
  bucket = var.vector_index_bucket_name
}

resource "aws_s3_bucket_versioning" "vector_index" {
  bucket = aws_s3_bucket.vector_index.id

  versioning_configuration {
    status = "Enabled"
  }
}

resource "aws_s3_bucket_server_side_encryption_configuration" "vector_index" {
  bucket = aws_s3_bucket.vector_index.id

  rule {
    apply_server_side_encryption_by_default {
      kms_master_key_id = aws_kms_key.artifacts.arn
      sse_algorithm     = "aws:kms"
    }
  }
}

resource "aws_s3_bucket_public_access_block" "platform_buckets" {
  for_each = {
    datasets       = aws_s3_bucket.datasets.id
    features       = aws_s3_bucket.features.id
    model_registry = aws_s3_bucket.model_registry.id
    vector_index   = aws_s3_bucket.vector_index.id
  }

  bucket                  = each.value
  block_public_acls       = true
  block_public_policy     = true
  ignore_public_acls      = true
  restrict_public_buckets = true
}
