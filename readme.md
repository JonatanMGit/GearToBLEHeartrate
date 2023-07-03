# ESP32 Heart Rate Monitor and BLE trasmitter

This project utilizes the ESP32 microcontroller in combination with the PlatformIO development platform to capture heart rate data from the [HeartRateToWeb project](https://github.com/loic2665/HeartRateToWeb) via HTTP. The captured heart rate data is then transmitted over the Bluetooth Low Energy (BLE) GATT (Generic Attribute Profile) to make it compatible with BLE-enabled devices such as Android phones and the Oculus Quest 2.

## Features:

- BLE GATT server
- Transmits heart rate data over BLE
- Easily available app for on the galaxy store thanks to the [HeartRateToWeb project](https://github.com/loic2665/HeartRateToWeb) project

## Required hardware:

- ESP32 microcontroller or similar BLE-enabled microcontroller
- Heart rate sensor (For example Samsung Gear S3) with the [HeartRateToWeb app](https://galaxystore.samsung.com/geardetail/tUhSWQRbmv)

## Installation

- [HeartRateToWeb app](https://galaxystore.samsung.com/geardetail/tUhSWQRbmv) on any compatible Smartwatch
- Install [PlatformIO](https://platformio.org/install/ide?install=vscode) on your computer
- Clone this repository
- Open the project in PlatformIO
- Connect your ESP32 to your computer
- Build and upload the project to your ESP32
- Open the [HeartRateToWeb app](https://galaxystore.samsung.com/geardetail/tUhSWQRbmv) on your smartwatch
- Enter the IP address of your ESP32 in the app (It is printed in the serial consle. The port is the default port 6547)
- Start the app and the heart rate data should be displayed on your smartwatch
- The ESP32 should now be visible as a BLE device and start transmitting heart rate data
- Connect a separate BLE device to the ESP32 to receive the heart rate data

## Contributing

Contributions to this project are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request on GitHub.
