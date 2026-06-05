variable "aws_region" {
  description = "AWS region for the platform."
  type        = string
  default     = "eu-central-1"
}

variable "project_name" {
  description = "Short project identifier used in resource names."
  type        = string
  default     = "ai-platform"
}

variable "environment" {
  description = "Environment name."
  type        = string
  default     = "prod"
}

variable "vpc_cidr" {
  description = "CIDR block for the VPC."
  type        = string
  default     = "10.30.0.0/16"
}

variable "private_subnet_cidrs" {
  description = "Private subnets for the EKS worker nodes."
  type        = list(string)
  default     = ["10.30.1.0/24", "10.30.2.0/24", "10.30.3.0/24"]
}

variable "public_subnet_cidrs" {
  description = "Public subnets for load balancers and NAT."
  type        = list(string)
  default     = ["10.30.101.0/24", "10.30.102.0/24", "10.30.103.0/24"]
}

variable "eks_cluster_version" {
  description = "Kubernetes version for the EKS control plane."
  type        = string
  default     = "1.29"
}

variable "node_instance_types" {
  description = "Worker node instance types."
  type        = list(string)
  default     = ["t3.large"]
}

variable "node_desired_size" {
  description = "Desired number of worker nodes."
  type        = number
  default     = 3
}

variable "node_min_size" {
  description = "Minimum number of worker nodes."
  type        = number
  default     = 2
}

variable "node_max_size" {
  description = "Maximum number of worker nodes."
  type        = number
  default     = 6
}

variable "dataset_bucket_name" {
  description = "Bucket for raw datasets that trigger retraining."
  type        = string
  default     = "otus-ai-platform-raw-datasets"
}

variable "feature_bucket_name" {
  description = "Bucket for prepared features and intermediate artifacts."
  type        = string
  default     = "otus-ai-platform-features"
}

variable "model_registry_bucket_name" {
  description = "Bucket for model registry artifacts and metadata snapshots."
  type        = string
  default     = "otus-ai-platform-model-registry"
}

variable "vector_index_bucket_name" {
  description = "Bucket for vector index snapshots used by RAG workloads."
  type        = string
  default     = "otus-ai-platform-vector-index"
}

variable "image_repository_name" {
  description = "ECR repository name for the application image."
  type        = string
  default     = "ai-service"
}

variable "tags" {
  description = "Additional tags applied to all resources."
  type        = map(string)
  default = {
    owner      = "otus"
    managed_by = "terraform"
    workload   = "ml-platform"
  }
}
