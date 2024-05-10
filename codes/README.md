# 실습용 코드 모음

기본적으로 nrf52840 DK 보드를 기준으로 실습 코드가 작성되어 있습니다.

## T0 (Prepare to develop)

[code](./t0_example)
- [LED Blinky](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/basic/blinky)
- GPIO를 이용해 nRF52840 DK 보드 LED를 깜빡이는 예제입니다.
- Build 환경 구축 및 개발 환경 설정 테스트를 위한 예제입니다.

## T1 (Zephyr Development with DeviceTree)

[code](./t1_example)
- [basic button](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/basic/button)
- 버튼을 눌러서 LED 상태를 변경하는 GPIO 기반 제어 예제입니다.

## T1_Practice (T1 Practice Solution)

- T1 예제의 실습 문제 해답 코드입니다.

[code](./t1_practice/buttoninterrupt_example)
- [button interrupt]
- 버튼 인터럽트 동작으로 LED 상태를 변경하는 GPIO 기반 제어 예제입니다.

[code](./t1_practice/pwmled_example)
- [PWM LED]
- 버튼 인터럽트 동작으로 PWM LED 상태를 변경하는 GPIO 기반 제어 예제입니다.

## T2 (Booting)

[code](./t2_example)
- [nRF System Off Demo](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/boards/nrf/system_off)
- 시스템 딥슬립 모드에 대한 내용을 담고 있습니다.
    - 저전력 딥슬립 모드로 들어갔다가 다시 깨어나는 과정에서 RAM 메모리 상의 데이터가 손실되지 않기 위한 RAM Retention 관련 실습이 가능합니다.

## T3 (UART)

[code](./t3_example)
- [CO2 Sensor]
- CO2 센서를 연겨랗여 센서 값을 표시하는 UART 통신 기반 예제입니다.

## T4 (BLE)

[code](./t4_example)
- [Bluetooth: Peripheral 예제](https://github.com/zephyrproject-rtos/zephyr/tree/v3.2.0/samples/bluetooth/peripheral)
- nRF Connect 앱을 통해 연결, 데이터 통신 값을 볼 수 있습니다.

## T5_1 (I2C LED2)

[code](./t5_1/ledmatrix_example)
- [HT16K33 LED driver with keyscan](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.5.2/zephyr/samples/drivers/ht16k33/README.html)
- 128개의 LED를 제어하는 HT16K33 드라이버 I2C통신 기반 예제입니다.

[code](./t5_1/secondscounter_example1)
- [Seconds Counter1]
- ledmatrix_example을 기반으로 시스템 타이머를 이용한 시간초 증가를 LED 표시하는 예제입니다.

[code](./t5_1/secondscounter_example2)
- [Seconds Counter2]
- Seconds Counter1 기반으로 시스템 타이머를 이용한 시간초 증가를 LED와 Battery Display를 통해 표시하는 예제입니다.

## T5_2 (Shield)

[code](./t5_2/joystick_example)
- [Joystick]
- ledmatrix_example을 기반으로 Joystick의 ADC값을 읽어 움직임을 LED로 표시하는 예제입니다.

[code](./t5_2/rotaryencoder_example)
- [Rotary Encoder]
- ledmatrix_example을 기반으로 Rotary Encoder의 회전 방향을 GPIO로 읽어 LED로 표시하는 예제입니다.

[code](./t5_2/soundsensor_example)
- [Sound Sensor]
- ledmatrix_example을 기반으로 Sound Sensor의 ADC값을 읽어 소리 크기를 LED로 표시하는 예제입니다.
