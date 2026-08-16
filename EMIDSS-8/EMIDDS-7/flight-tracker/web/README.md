# EMIDSS-8 Mission Control Web

React + TypeScript + Tailwind + Mapbox mission-control dashboard. Displays the
flight's live position on a map, current sensor readings, atmospheric charts
(temperature/humidity/pressure decoded from the SBD payload), and a raw
telemetry feed. Reactive layout for both desktop (chase vehicle) and mobile
(recovery team phones).

Data flow:
- **Historical**: polls `GET {VITE_API_BASE_URL}/telemetry` every 15s.
- **Live**: connects directly to AWS IoT Core over MQTT/WSS using guest
  credentials from Cognito (`src/lib/iotSigner.ts`), and subscribes to the
  live-tracking topic for instant updates.

## Setup

```bash
cp .env.example .env   # fill in values from `terraform output` in ../infra
npm install
npm run dev
```

## Structure

- `src/hooks/useTelemetryHistory.ts` — REST polling for historical records
- `src/hooks/useLiveTelemetry.ts` — MQTT/WSS subscription for live updates
- `src/lib/iotSigner.ts` — SigV4-presigns the IoT Core WebSocket URL
- `src/components/MapView.tsx` — Mapbox GL live position + flight track
- `src/components/AtmosphericCharts.tsx` — temperature/humidity/pressure charts
- `src/components/TelemetryTable.tsx` — raw record feed

## Deploy

Build and sync to the S3 bucket from `../infra` (`web_app_bucket_name`
output), then invalidate CloudFront (`cloudfront_distribution_id` output):

```bash
npm run build
aws s3 sync dist/ s3://<web_app_bucket_name> --delete
aws cloudfront create-invalidation --distribution-id <cloudfront_distribution_id> --paths "/*"
```
