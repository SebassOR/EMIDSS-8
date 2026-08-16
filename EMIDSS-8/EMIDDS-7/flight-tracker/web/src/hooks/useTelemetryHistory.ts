import { useEffect, useState } from "react";
import { config } from "../lib/config";
import type { TelemetryRecord } from "../types";

interface TelemetryHistoryState {
  flightId: string | null;
  records: TelemetryRecord[];
  loading: boolean;
  error: string | null;
}

export function useTelemetryHistory(pollIntervalMs = 15000): TelemetryHistoryState {
  const [state, setState] = useState<TelemetryHistoryState>({
    flightId: null,
    records: [],
    loading: true,
    error: null,
  });

  useEffect(() => {
    let cancelled = false;

    async function fetchHistory() {
      try {
        const res = await fetch(`${config.apiBaseUrl}/telemetry`);
        if (!res.ok) throw new Error(`Request failed: ${res.status}`);
        const body = await res.json();
        if (!cancelled) {
          setState({
            flightId: body.flight_id,
            records: body.records ?? [],
            loading: false,
            error: null,
          });
        }
      } catch (err) {
        if (!cancelled) {
          setState((prev) => ({
            ...prev,
            loading: false,
            error: err instanceof Error ? err.message : "Failed to load telemetry",
          }));
        }
      }
    }

    fetchHistory();
    const interval = setInterval(fetchHistory, pollIntervalMs);
    return () => {
      cancelled = true;
      clearInterval(interval);
    };
  }, [pollIntervalMs]);

  return state;
}
