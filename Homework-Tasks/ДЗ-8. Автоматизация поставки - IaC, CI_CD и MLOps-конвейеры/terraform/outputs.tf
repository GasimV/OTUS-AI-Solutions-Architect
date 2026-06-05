output "eks_cluster_name" {
  description = "Name of the Kubernetes cluster."
  value       = module.eks.cluster_name
}

output "eks_cluster_endpoint" {
  description = "API server endpoint for the EKS cluster."
  value       = module.eks.cluster_endpoint
}

output "ecr_repository_url" {
  description = "Docker repository URL for the application."
  value       = aws_ecr_repository.app.repository_url
}

output "dataset_bucket_name" {
  description = "Bucket containing raw datasets."
  value       = aws_s3_bucket.datasets.id
}

output "feature_bucket_name" {
  description = "Bucket with prepared features."
  value       = aws_s3_bucket.features.id
}

output "model_registry_bucket_name" {
  description = "Bucket containing model artifacts and registry snapshots."
  value       = aws_s3_bucket.model_registry.id
}

output "vector_index_bucket_name" {
  description = "Bucket storing vector index snapshots for RAG."
  value       = aws_s3_bucket.vector_index.id
}
