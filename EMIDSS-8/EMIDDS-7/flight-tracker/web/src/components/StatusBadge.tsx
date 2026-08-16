import { Radio, RadioTower, WifiOff } from "lucide-react";
import type { ConnectionStatus } from "../types";

const CONFIG: Record<ConnectionStatus, { label: string; color: string; Icon: typeof Radio }> = {
  live: { label: "Live", color: "var(--status-good)", Icon: RadioTower },
  connecting: { label: "Connecting", color: "var(--status-warning)", Icon: Radio },
  offline: { label: "Offline", color: "var(--status-critical)", Icon: WifiOff },
};

export function StatusBadge({ status }: { status: ConnectionStatus }) {
  const { label, color, Icon } = CONFIG[status];
  return (
    <span className="inline-flex items-center gap-1.5 rounded-full border border-zinc-300 bg-zinc-100 px-3 py-1 text-xs font-medium text-zinc-700 dark:border-zinc-800 dark:bg-zinc-900/80 dark:text-zinc-300">
      <Icon size={13} style={{ color }} aria-hidden />
      {label}
    </span>
  );
}
