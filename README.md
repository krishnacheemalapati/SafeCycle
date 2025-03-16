# SafeCycle

SafeCycle is a revolutionary application designed to enhance bicycle safety through real-time proximity detection. This project was submitted for WallyHacks 2025.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [API Endpoints](#api-endpoints)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## Introduction

SafeCycle aims to provide cyclists with real-time data on their surroundings, helping them stay safe on the road. The application collects sensor data, processes it, and displays it on a live dashboard. It also provides audio and visual alerts when obstacles are detected within unsafe distances.

## Features

- Real-time proximity detection using ultrasonic sensors
- Live dashboard for monitoring sensor data
- WebSocket integration for real-time updates
- REST API for accessing sensor data
- Audio and LED alerts for distance-based warnings
- Moving average filter for smoother sensor readings

## Installation

To get started with SafeCycle, follow these steps:

### Clone the repository:

```bash
git clone https://github.com/yourusername/SafeCycle.git
cd SafeCycle
```

### Install the required dependencies:

```bash
pip install -r requirements.txt  # For backend
npm install  # For frontend
```

### Set up the database (if applicable):

```bash
python manage.py migrate  # If using Django
```

### Start the backend server:

```bash
python manage.py runserver
```

### Start the frontend development server:

```bash
npm start
```

Access the dashboard at http://localhost:8000

## Usage

To use SafeCycle, ensure your hardware setup is connected and running. Open the web dashboard to monitor sensor readings in real-time. Alerts will trigger based on detected distances.

## API Endpoints

### Get Latest Readings

- **URL:** `/api/latest/`
- **Method:** `GET`
- **Description:** Retrieves the latest 10 sensor readings.

### Create a New Sensor Reading

- **URL:** `/api/sensor-readings/`
- **Method:** `POST`
- **Description:** Creates a new sensor reading.
- **Body Example:**

```json
{
  "left_distance": 25.5,
  "right_distance": 30.2,
  "back_distance": 15.7,
  "timestamp": "2025-03-15T12:00:00Z",
  "alert_level": "low",
  "device_id": "device123"
}
```

### Get All Sensor Readings

- **URL:** `/api/sensor-readings/`
- **Method:** `GET`
- **Description:** Retrieves all sensor readings.


## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-name`).
3. Commit your changes (`git commit -m "Add feature"`).
4. Push to your fork and submit a pull request.

## License

This project is licensed under the MIT License. See LICENSE for details.
