import {
  CategoryScale,
  Chart as ChartJS,
  Legend,
  LineElement,
  LinearScale,
  PointElement,
  Tooltip,
} from "chart.js";
import { Line } from "react-chartjs-2";

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Tooltip,
  Legend
);

type TelemetryLineChartProps = {
  title: string;
  labels: string[];
  values: number[];
  unit?: string;
  borderColor: string;
  backgroundColor: string;
};

export function TelemetryLineChart({
  title,
  labels,
  values,
  unit,
  borderColor,
  backgroundColor,
}: TelemetryLineChartProps) {
  const chartData = {
    labels,
    datasets: [
      {
        label: unit ? `${title} (${unit})` : title,
        data: values,
        borderColor,
        backgroundColor,
        tension: 0.3,
        pointRadius: 2,
        pointHoverRadius: 5,
      },
    ],
  };

  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    animation: {
      duration: 350,
    },
    plugins: {
      legend: {
        labels: {
          color: "#cbd5e1",
        },
      },
      tooltip: {
        mode: "index" as const,
        intersect: false,
      },
    },
    scales: {
      x: {
        ticks: {
          color: "#94a3b8",
          maxRotation: 0,
          autoSkip: true,
          maxTicksLimit: 8,
        },
        grid: {
          color: "#1e293b",
        },
      },
      y: {
        ticks: {
          color: "#94a3b8",
        },
        grid: {
          color: "#1e293b",
        },
      },
    },
  };

  return (
    <div className="rounded-xl border border-slate-800 bg-slate-950 p-4">
      <div className="mb-3">
        <h3 className="text-base font-semibold text-white">{title}</h3>
        <p className="text-xs text-slate-500">
          {values.length} telemetry points received.
        </p>
      </div>

      <div className="h-72">
        <Line data={chartData} options={chartOptions} />
      </div>
    </div>
  );
}