"""Serves telemetry history to the mission-control web app.

GET /telemetry                      -> most recent flight's records
GET /telemetry?flight_id=<imei>     -> a specific flight's records
GET /telemetry?flight_id=..&limit=N -> cap record count (default 200, max 1000)
"""
import json
import os
from decimal import Decimal

import boto3
from boto3.dynamodb.conditions import Key

dynamodb = boto3.resource("dynamodb")
table = dynamodb.Table(os.environ["DYNAMODB_TABLE"])

CORS_HEADERS = {
    "Content-Type": "application/json",
    "Access-Control-Allow-Origin": os.environ.get("CORS_ALLOW_ORIGIN", "*"),
}


def lambda_handler(event, context):
    params = event.get("queryStringParameters") or {}
    limit = min(_to_int(params.get("limit")) or 200, 1000)

    flight_id = params.get("flight_id") or _latest_flight_id()
    if not flight_id:
        return _response(200, {"flight_id": None, "records": []})

    result = table.query(
        KeyConditionExpression=Key("flight_id").eq(flight_id),
        ScanIndexForward=False,
        Limit=limit,
    )
    records = sorted(result.get("Items", []), key=lambda r: r["timestamp"])
    return _response(200, {"flight_id": flight_id, "records": records})


def _latest_flight_id():
    result = table.scan(Limit=1)
    items = result.get("Items")
    return items[0]["flight_id"] if items else None


def _to_int(value):
    try:
        return int(value)
    except (TypeError, ValueError):
        return None


def _default(value):
    if isinstance(value, Decimal):
        return float(value)
    raise TypeError(f"not JSON serializable: {value!r}")


def _response(status, body):
    return {
        "statusCode": status,
        "headers": CORS_HEADERS,
        "body": json.dumps(body, default=_default),
    }
