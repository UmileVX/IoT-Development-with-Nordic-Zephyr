# IoT-Development-with-Nordic-Zephyr
한동대학교 마이크로프로세서 응용 과목 교재 개발을 위한 자료 및 링크 모음입니다.
(주)테크노니아의 IoT 시스템 개발 플랫폼의 자료실의 역할도 합니다.

## ARM 아키텍쳐

참고 1) 시스템 소프트웨어 개발을 위한 Arm아키텍쳐의 구조와 원리
 https://www.aladin.co.kr/shop/wproduct.aspx?ItemId=318773448&start=slayer
 
 저자 직강 (유료, Udemy)
 https://www.udemy.com/course/austin-armv8_v7_arch1/?referralCode=4D1703A5611F14D1FD50
 
 저자 블로그 : https://austindhkim.tistory.com/

## Zephyr RTOS + Nordic

- Linux Foundation의 [Zephyr RTOS](https://zephyrproject.org/)를 사용합니다.
    - 오픈소스 기반의 RTOS
    - [GitHub Repository](https://github.com/zephyrproject-rtos/zephyr)
    - VSCode 확장을 통해 간편하게 빌드 및 플래시 가능
    - 임베디드 하드웨어를 코드로서 정의 및 구현 가능
        - [Device Tree](https://docs.zephyrproject.org/latest/build/dts/index.html)를 통해 하드웨어를 자료구조로 정의
        - 컴파일 시에 하드웨어 스펙을 코드로서 함께 컴파일
    - Nordic, Adafruit, Google, 퀄컴 등 수많은 기업들에서 표준 환경으로 채택
        - [구글 크롬북에 Zephyr RTOS를 사용](https://zephyrproject.org/chromeoss-embedded-controller/)
        - Nordic 신규 SDK에서는 Zephyr API를 표준으로 설정

- Nordic
    - Nordic사의 nRF5340 혹은 nRF52840을 사용합니다.
    - [nrf7002 DK 보드](https://www.nordicsemi.com/Products/Development-hardware/nRF7002-DK)

### 자료

1. [nrf SDK에서 Zephyr RTOS 사용법](./docs/nrf_sdk_zephyr_basic.md)
