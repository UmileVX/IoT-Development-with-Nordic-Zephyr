# 실습용 코드 모음

기본적으로 nrf52840 DK 보드를 기준으로 실습 코드가 작성되어 있습니다.

## SystemOff

- [nRF System Off Demo](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/boards/nrf/system_off) 기반으로 작성됨
- 시스템 딥슬립 모드에 대한 내용을 담고 있음.
    - 저전력 딥슬립 모드로 들어갔다가 다시 깨어나는 과정에서 RAM 메모리 상의 데이터가 손실되지 않기 위한 RAM Retention 관련 실습이 가능
