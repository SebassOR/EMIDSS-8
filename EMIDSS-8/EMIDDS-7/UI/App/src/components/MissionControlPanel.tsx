import { useState } from "react";
import { invoke } from "@tauri-apps/api/core";

type CommandAction = "ReadMemory" | "ReadTime" | "ResetSensor";

const missionCommands: { label: string; action: CommandAction }[] = [
  { label: "Read Memory", action: "ReadMemory" },
  { label: "Read Time", action: "ReadTime" },
  { label: "Reset Sensor", action: "ResetSensor" },
];

export function MissionControlPanel() {
  const [activeCommand, setActiveCommand] = useState<CommandAction | null>(null);
  const [statusMessage, setStatusMessage] = useState("Ready to send mission commands.");
  const [errorMessage, setErrorMessage] = useState("");

  async function handleCommand(action: CommandAction) {
    setActiveCommand(action);
    setErrorMessage("");
    setStatusMessage(`Sending ${action} command...`);

    try {
      // Send semantic command to Rust. Rust maps it to the UART protocol
      await invoke("send_command", { action });

      setStatusMessage(`${action} command sent successfully.`);
    } catch (error) {
      setErrorMessage(String(error));
      setStatusMessage("Command failed.");
    } finally {
      setActiveCommand(null);
    }
  }

  return (
    <section className="rounded-2xl border border-slate-700 bg-slate-900 p-5 shadow-lg">
      <div className="mb-4">
        <h2 className="text-lg font-semibold text-white">Mission Control Panel</h2>
        <p className="text-sm text-slate-400">
          Send mission commands to the Rust backend through Tauri IPC
        </p>
      </div>

      <div className="grid gap-3 md:grid-cols-3">
        {missionCommands.map((command) => (
          <button
            key={command.action}
            onClick={() => handleCommand(command.action)}
            disabled={activeCommand !== null}
            className="rounded-lg border border-slate-700 bg-slate-950 px-4 py-3 font-medium text-slate-100 transition hover:border-blue-500 hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
          >
            {activeCommand === command.action ? "Sending..." : command.label}
          </button>
        ))}
      </div>

      <p className="mt-4 text-sm text-slate-300">
        Status: <span className="text-white">{statusMessage}</span>
      </p>

      {errorMessage && (
        <p className="mt-3 rounded-lg border border-red-800 bg-red-950/50 px-3 py-2 text-sm text-red-300">
          {errorMessage}
        </p>
      )}
    </section>
  );
}