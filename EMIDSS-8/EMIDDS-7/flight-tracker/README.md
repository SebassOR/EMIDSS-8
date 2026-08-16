# Flight Tracker

Ingests EMIDSS-8 flight telemetry from the Rock7/RockBLOCK SBD webhook and
displays it live on a mission-control web dashboard.

- [`infra/`](infra) — Terraform for the AWS backend (API Gateway, Lambda,
  DynamoDB, IoT Core, S3 + CloudFront)
- [`api/`](api) — Lambda application code (webhook ingest, telemetry read API)
- [`web/`](web) — React + Tailwind + Mapbox mission-control dashboard
