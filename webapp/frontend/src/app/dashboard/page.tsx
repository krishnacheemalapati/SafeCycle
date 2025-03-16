'use client';

import { useEffect, useState } from 'react';
import { Line } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  ChartData
} from 'chart.js';

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend
);

interface SensorReading {
  timestamp: string;
  left_distance: number;
  right_distance: number;
  back_distance: number;
  alert_level: string;
  device_id: string;
}

export default function Dashboard() {
  const [readings, setReadings] = useState<SensorReading[]>([]);
  const [ws, setWs] = useState<WebSocket | null>(null);

  useEffect(() => {
    // Fetch initial data
    fetch('http://localhost:8000/api/readings/latest/')
      .then(res => res.json())
      .then(data => setReadings(data));

    // Setup WebSocket connection
    const websocket = new WebSocket('ws://localhost:8000/ws/sensor_data/');
    websocket.onmessage = (event) => {
      const data = JSON.parse(event.data);
      setReadings(prev => [...prev, data].slice(-50)); // Keep last 50 readings
    };
    setWs(websocket);

    return () => {
      if (ws) ws.close();
    };
  }, []);

  const chartData: ChartData<'line'> = {
    labels: readings.map(r => new Date(r.timestamp).toLocaleTimeString()),
    datasets: [
      {
        label: 'Left Distance',
        data: readings.map(r => r.left_distance),
        borderColor: 'rgb(75, 192, 192)',
        tension: 0.1
      },
      {
        label: 'Right Distance',
        data: readings.map(r => r.right_distance),
        borderColor: 'rgb(255, 99, 132)',
        tension: 0.1
      },
      {
        label: 'Back Distance',
        data: readings.map(r => r.back_distance),
        borderColor: 'rgb(153, 102, 255)',
        tension: 0.1
      }
    ]
  };

  const chartOptions = {
    responsive: true,
    plugins: {
      legend: {
        position: 'top' as const,
      },
      title: {
        display: true,
        text: 'Sensor Readings Over Time'
      }
    },
    scales: {
      y: {
        beginAtZero: true,
        title: {
          display: true,
          text: 'Distance (cm)'
        }
      }
    }
  };

  return (
    <div className="min-h-screen bg-gray-900 text-white p-8">
      <h1 className="text-4xl font-bold mb-8">SafeCycle Dashboard</h1>
      
      {/* Real-time Chart */}
      <div className="bg-gray-800 p-6 rounded-lg mb-8">
        <Line data={chartData} options={chartOptions} />
      </div>

      {/* Latest Readings */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
        <div className="bg-gray-800 p-6 rounded-lg">
          <h3 className="text-xl font-bold mb-4">Left Sensor</h3>
          <p className="text-4xl font-bold">
            {readings[readings.length - 1]?.left_distance.toFixed(1) || '-'} cm
          </p>
        </div>
        <div className="bg-gray-800 p-6 rounded-lg">
          <h3 className="text-xl font-bold mb-4">Right Sensor</h3>
          <p className="text-4xl font-bold">
            {readings[readings.length - 1]?.right_distance.toFixed(1) || '-'} cm
          </p>
        </div>
        <div className="bg-gray-800 p-6 rounded-lg">
          <h3 className="text-xl font-bold mb-4">Back Sensor</h3>
          <p className="text-4xl font-bold">
            {readings[readings.length - 1]?.back_distance.toFixed(1) || '-'} cm
          </p>
        </div>
      </div>
    </div>
  );
} 