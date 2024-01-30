# AB02S SD Card Read/Write Test

## Introduction
This project, `ab02s_sd`, is designed to test SD card read/write operations using the Heltec AB02S development board. It demonstrates how to integrate the Heltec CubeCell library with an SD card library to perform these operations.

## Prerequisites
Before you start, ensure you have the following:

- Heltec AB02S Development Board
- An SD card and SD card module compatible with the Heltec AB02S
- Arduino IDE or a similar development environment

## Libraries Required
To successfully implement this project, you need to include two primary libraries:

1. **Heltec CubeCell Library:** Necessary for interfacing with the Heltec AB02S board.
2. **SD Library:** This library enables SD card read/write functionality.

## Installation
Follow these steps to set up your environment:

1. **Install Arduino IDE:** Download and install Arduino IDE from [the official website](https://www.arduino.cc/en/software).

2. **Add Heltec CubeCell Board to Arduino IDE:**
   - In Arduino IDE, go to `File > Preferences`.
   - Under 'Additional Board Manager URLs', add the following URL: `https://resource.heltec.cn/download/package_CubeCell_index.json`
   - Open `Tools > Board > Boards Manager`, search for `CubeCell`, and install the latest version.

3. **Install SD Library:**
   - Go to `Sketch > Include Library > Manage Libraries...` in Arduino IDE.
   - Search for `SD` and install the library.

## Configuration
Make sure to configure your project correctly:

- Include both the Heltec CubeCell and SD libraries in your sketch.
- Configure the pinouts and settings according to your specific SD card module and the AB02S board.

## Usage
To use this project:

1. Connect your SD card module to the Heltec AB02S board.
2. Open the provided sketch in Arduino IDE.
3. Modify the sketch if necessary to suit your specific hardware setup.
4. Upload the sketch to the AB02S board.
5. Monitor the serial output to see the results of the SD card read/write tests.

## Contributing
Contributions to `ab02s_sd` are welcome. Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -am 'Add some feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a new Pull Request.

## License
This project is licensed under [MIT License](LICENSE).

## Acknowledgements
- Heltec for providing the CubeCell library and development board.
- Contributors and maintainers of the SD library.

---

For more information and support, please open an issue in the GitHub repository.

