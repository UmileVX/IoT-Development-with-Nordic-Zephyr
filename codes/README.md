# 실습용 코드 모음

기본적으로 nrf52840 DK 보드를 기준으로 실습 코드가 작성되어 있습니다.

## SystemOff

- [nRF System Off Demo](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/boards/nrf/system_off) 기반으로 작성됨
- 시스템 딥슬립 모드에 대한 내용을 담고 있음.
    - 저전력 딥슬립 모드로 들어갔다가 다시 깨어나는 과정에서 RAM 메모리 상의 데이터가 손실되지 않기 위한 RAM Retention 관련 실습이 가능

## CO2_bot

- [UART echo_bot](https://github.com/zephyrproject-rtos/zephyr/tree/v3.2.0/samples/drivers/uart/echo_bot) 기반으로 작성됨
- 기존 UART 포트를 커스텀 포트로 오버라이딩하고, 설정한 포트를 기반으로 UART 통신을 하는 방법을 학습

## Practice1

- 테크노니아에서 작성한 lvgl 기반의 단순 예제
- GPIO 포트들을 사용해서 SPI 통신을 설정
- SPI 기반 통신을 통해 LCD 제어하는 방법을 학습

## peripheral

- [Bluetooth: Peripheral 예제](https://github.com/zephyrproject-rtos/zephyr/tree/v3.2.0/samples/bluetooth/peripheral) 기반
- nRF Connect 앱을 통해 연결, 데이터 통신 값을 볼 수 있음
