import "./App.css";
import { ConnectionManager } from "./components/ConnectionManager";
import { MissionControlPanel } from "./components/MissionControlPanel";
import { TelemetryDashboard } from "./components/TelemetryDashboard";
import { TerminalConsole } from "./components/TerminalConsole";

function App() {
  return (
    <main className="min-h-screen bg-slate-950 px-6 py-6 text-slate-100">
      <div className="mx-auto flex max-w-7xl flex-col gap-6">
        <header className="rounded-2xl border border-slate-800 bg-slate-900 p-6 shadow-lg">
          <p className="text-sm font-medium uppercase tracking-[0.3em] text-blue-400">
            EMIDSS-8
          </p>

          <h1 className="mt-2 text-3xl font-bold text-white">
            Telemetry Analysis Application
          </h1>

          <p className="mt-2 max-w-3xl text-slate-400">
            UART connection management, mission commands, live telemetry charts,
            and Rust log monitoring.
          </p>
        </header>

        <ConnectionManager />

        <MissionControlPanel />

        <TelemetryDashboard />

        <TerminalConsole />
      </div>
    </main>
  );
}

export default App;