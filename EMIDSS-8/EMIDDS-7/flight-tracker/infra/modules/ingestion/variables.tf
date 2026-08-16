variable "name_prefix" {
  type = string
}

variable "aws_region" {
  type = string
}

variable "account_id" {
  type = string
}

variable "iot_topic" {
  type = string
}

variable "lambda_runtime" {
  type    = string
  default = "python3.12"
}

variable "log_retention_days" {
  type    = number
  default = 14
}

variable "cloudfront_domain_name" {
  description = "The web app's CloudFront domain, allowed to call GET /telemetry from a browser."
  type        = string
}

variable "enable_dynamodb_pitr" {
  description = "Point-in-time recovery adds ~$0.20/GB-month; off by default since telemetry can be re-derived from Rock7's own delivery history."
  type        = bool
  default     = false
}

variable "dynamodb_ttl_days" {
  description = "Records older than this are auto-deleted by DynamoDB TTL, so repeated local test flights don't accumulate storage cost forever."
  type        = number
  default     = 90
}
