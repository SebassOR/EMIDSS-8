import mapboxgl from "mapbox-gl";
import "mapbox-gl/dist/mapbox-gl.css";
import type { Feature } from "geojson";
import { Layers, Satellite } from "lucide-react";
import { useEffect, useRef, useState } from "react";
import { circlePolygon } from "../lib/geo";
import { config } from "../lib/config";
import { escapeHtml } from "../lib/html";
import type { Theme } from "../hooks/useTheme";
import type { TelemetryRecord } from "../types";

const UNCERTAINTY_SOURCE_ID = "uncertainty-circles";
const UNCERTAINTY_FILL_LAYER_ID = "uncertainty-circles-fill";
const UNCERTAINTY_LINE_LAYER_ID = "uncertainty-circles-line";
const POINTS_SOURCE_ID = "position-points";
const POINTS_LAYER_ID = "position-points-circle";

const DEFAULT_UNCERTAINTY_M = 500;
type Basemap = "flat" | "satellite";
const STYLE_URL: Record<Theme, string> = {
  dark: "mapbox://styles/mapbox/dark-v11",
  light: "mapbox://styles/mapbox/light-v11",
};
const SATELLITE_STYLE_URL = "mapbox://styles/mapbox/satellite-streets-v12";

function resolveStyle(theme: Theme, basemap: Basemap): string {
  return basemap === "satellite" ? SATELLITE_STYLE_URL : STYLE_URL[theme];
}

interface PointProperties {
  isLatest: boolean;
  momsn?: number;
  transmit_time?: string;
  latitude: number;
  longitude: number;
  cep?: number;
  session_status?: number;
  decoded_text?: string;
}

function toFeatures(positioned: TelemetryRecord[]): { circles: Feature[]; points: Feature[] } {
  const circles: Feature[] = positioned.map((r, i) => {
    const center: [number, number] = [r.longitude as number, r.latitude as number];
    const radiusMeters = (r.cep ? r.cep * 1000 : DEFAULT_UNCERTAINTY_M) || DEFAULT_UNCERTAINTY_M;
    return {
      type: "Feature",
      geometry: circlePolygon(center, radiusMeters),
      properties: { isLatest: i === positioned.length - 1 },
    };
  });

  const points: Feature[] = positioned.map((r, i) => {
    const properties: PointProperties = {
      isLatest: i === positioned.length - 1,
      momsn: r.momsn,
      transmit_time: r.transmit_time,
      latitude: r.latitude as number,
      longitude: r.longitude as number,
      cep: r.cep,
      session_status: r.session_status,
      decoded_text: r.decoded_text,
    };
    return {
      type: "Feature",
      geometry: { type: "Point", coordinates: [r.longitude as number, r.latitude as number] },
      properties,
    };
  });

  return { circles, points };
}

function popupHtml(p: PointProperties): string {
  return `
    <div class="min-w-[190px] text-xs">
      <div class="mb-1.5 font-semibold text-sm">MOMSN ${escapeHtml(p.momsn)}</div>
      <div class="space-y-1 opacity-80">
        <div>Transmit: ${escapeHtml(p.transmit_time)}</div>
        <div>Lat/Lon: ${p.latitude.toFixed(4)}, ${p.longitude.toFixed(4)}</div>
        <div>CEP: ${p.cep !== undefined ? `${p.cep} km` : "—"}</div>
        <div>Session status: ${escapeHtml(p.session_status)}</div>
      </div>
      ${
        p.decoded_text
          ? `<div class="mt-1.5 pt-1.5 font-mono opacity-70" style="border-top:1px solid rgba(128,128,128,0.35)">${escapeHtml(p.decoded_text)}</div>`
          : ""
      }
    </div>`;
}

/**
 * SBD deliveries arrive roughly every 5-10 minutes and Iridium's own CEP
 * (Circular Error Probable) is typically a couple of kilometers, so a
 * connected polyline overstates precision. Instead every fix is drawn as an
 * independent point with a translucent uncertainty circle around it; click a
 * point to see that fix's full telemetry.
 */
export function MapView({ records, theme }: { records: TelemetryRecord[]; theme: Theme }) {
  const containerRef = useRef<HTMLDivElement>(null);
  const mapRef = useRef<mapboxgl.Map | null>(null);
  const markerRef = useRef<mapboxgl.Marker | null>(null);
  const positionedRef = useRef<TelemetryRecord[]>([]);
  const currentStyleRef = useRef<string | null>(null);
  const [basemap, setBasemap] = useState<Basemap>("flat");

  const positioned = records.filter(
    (r) => typeof r.latitude === "number" && typeof r.longitude === "number",
  );
  positionedRef.current = positioned;
  const latest = positioned[positioned.length - 1];

  useEffect(() => {
    if (!containerRef.current || mapRef.current) return;
    if (!config.mapboxToken) return;

    mapboxgl.accessToken = config.mapboxToken;
    const map = new mapboxgl.Map({
      container: containerRef.current,
      style: resolveStyle(theme, basemap),
      center: [0, 0],
      zoom: 2,
    });
    map.addControl(new mapboxgl.NavigationControl(), "top-right");
    mapRef.current = map;
    currentStyleRef.current = resolveStyle(theme, basemap);

    const syncLayers = () => {
      if (!map.getSource(UNCERTAINTY_SOURCE_ID)) {
        map.addSource(UNCERTAINTY_SOURCE_ID, {
          type: "geojson",
          data: { type: "FeatureCollection", features: [] },
        });
        map.addLayer({
          id: UNCERTAINTY_FILL_LAYER_ID,
          type: "fill",
          source: UNCERTAINTY_SOURCE_ID,
          paint: {
            "fill-color": "#3987e5",
            "fill-opacity": ["case", ["get", "isLatest"], 0.16, 0.06],
          },
        });
        map.addLayer({
          id: UNCERTAINTY_LINE_LAYER_ID,
          type: "line",
          source: UNCERTAINTY_SOURCE_ID,
          paint: {
            "line-color": "#3987e5",
            "line-width": ["case", ["get", "isLatest"], 1.5, 0.75],
            "line-opacity": ["case", ["get", "isLatest"], 0.7, 0.3],
          },
        });
      }

      if (!map.getSource(POINTS_SOURCE_ID)) {
        map.addSource(POINTS_SOURCE_ID, {
          type: "geojson",
          data: { type: "FeatureCollection", features: [] },
        });
        map.addLayer({
          id: POINTS_LAYER_ID,
          type: "circle",
          source: POINTS_SOURCE_ID,
          paint: {
            "circle-radius": ["case", ["get", "isLatest"], 6, 4],
            "circle-color": "#3987e5",
            "circle-opacity": ["case", ["get", "isLatest"], 1, 0.6],
            "circle-stroke-width": 1,
            "circle-stroke-color": "#ffffff",
          },
        });
      }

      const { circles, points } = toFeatures(positionedRef.current);
      (map.getSource(UNCERTAINTY_SOURCE_ID) as mapboxgl.GeoJSONSource)?.setData({
        type: "FeatureCollection",
        features: circles,
      });
      (map.getSource(POINTS_SOURCE_ID) as mapboxgl.GeoJSONSource)?.setData({
        type: "FeatureCollection",
        features: points,
      });
    };

    // "style.load" fires as soon as the new style JSON is applied - do NOT
    // gate this on isStyleLoaded() (tiles/sources may still be loading, so
    // it's often still false here), or the re-add silently gets skipped and
    // the trail vanishes on every theme/basemap switch.
    map.on("load", syncLayers);
    map.on("style.load", syncLayers);

    const popup = new mapboxgl.Popup({ closeButton: true, closeOnClick: true, offset: 10 });
    map.on("click", POINTS_LAYER_ID, (e) => {
      const feature = e.features?.[0];
      if (!feature || feature.geometry.type !== "Point") return;
      const coords = feature.geometry.coordinates.slice() as [number, number];
      popup.setLngLat(coords).setHTML(popupHtml(feature.properties as PointProperties)).addTo(map);
    });
    map.on("mouseenter", POINTS_LAYER_ID, () => {
      map.getCanvas().style.cursor = "pointer";
    });
    map.on("mouseleave", POINTS_LAYER_ID, () => {
      map.getCanvas().style.cursor = "";
    });

    return () => {
      map.remove();
      mapRef.current = null;
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  // Theme/basemap switch: swap the base style; syncLayers() re-adds data via style.load.
  // Only call setStyle() when the resolved style actually differs from what
  // this map instance currently has - comparing against a "did we already
  // run once" flag instead breaks under React StrictMode's dev-mode double
  // mount (the map gets destroyed and recreated, but a plain useRef flag
  // survives that remount), which would call setStyle on a brand-new map
  // before its initial "load" fires and silently wipe the layers.
  useEffect(() => {
    const map = mapRef.current;
    if (!map) return;
    const nextStyle = resolveStyle(theme, basemap);
    if (currentStyleRef.current === nextStyle) return;
    currentStyleRef.current = nextStyle;
    map.setStyle(nextStyle);
  }, [theme, basemap]);

  useEffect(() => {
    const map = mapRef.current;
    if (!map || positioned.length === 0) return;

    const applyUpdate = () => {
      const { circles, points } = toFeatures(positioned);
      const circleSource = map.getSource(UNCERTAINTY_SOURCE_ID) as mapboxgl.GeoJSONSource | undefined;
      circleSource?.setData({ type: "FeatureCollection", features: circles });
      const pointSource = map.getSource(POINTS_SOURCE_ID) as mapboxgl.GeoJSONSource | undefined;
      pointSource?.setData({ type: "FeatureCollection", features: points });
    };
    if (map.getSource(POINTS_SOURCE_ID)) applyUpdate();

    const lngLat: [number, number] = [latest.longitude as number, latest.latitude as number];
    if (!markerRef.current) {
      const el = document.createElement("div");
      el.className = "h-3 w-3 rounded-full bg-blue-400 ring-2 ring-blue-400/60 animate-pulse";
      markerRef.current = new mapboxgl.Marker({ element: el }).setLngLat(lngLat).addTo(map);
      map.jumpTo({ center: lngLat, zoom: 11 });
    } else {
      markerRef.current.setLngLat(lngLat);
      map.easeTo({ center: lngLat, duration: 800 });
    }
  }, [positioned, latest]);

  if (!config.mapboxToken) {
    return (
      <div className="flex h-full items-center justify-center bg-zinc-100 text-sm text-zinc-500 dark:bg-zinc-900">
        Set VITE_MAPBOX_TOKEN to enable the live map.
      </div>
    );
  }

  return (
    <div className="relative h-full w-full">
      <div ref={containerRef} className="h-full w-full" />
      <button
        onClick={() => setBasemap((m) => (m === "satellite" ? "flat" : "satellite"))}
        className="absolute right-4 bottom-16 z-10 flex items-center gap-1.5 rounded-lg border border-zinc-300 bg-white/90 px-2.5 py-1.5 text-xs font-medium text-zinc-700 shadow-sm backdrop-blur hover:text-zinc-900 dark:border-zinc-800 dark:bg-zinc-950/90 dark:text-zinc-300 dark:shadow-none dark:hover:text-zinc-100"
      >
        {basemap === "satellite" ? (
          <>
            <Layers size={14} /> Map
          </>
        ) : (
          <>
            <Satellite size={14} /> Satellite
          </>
        )}
      </button>
    </div>
  );
}
