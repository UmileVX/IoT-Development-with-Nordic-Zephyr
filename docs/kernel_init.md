# Zephyr RTOS Kernel Init

## Init 순서

Zephyr에서 sys_init을 통해 초기 작업의 순서에 대해서 정의하는 기능을 구현하고 있었고, 이 기능의 동작 방식에 대해서 깊이 있게 이해하기 위해 debug tool을 통해 sys_init level 별 실행 순서에 대해서 알아보았다.
이 과정을 진행한 기본적인 이유는, sys_init을 통해 정의할 수 있는 레벨이 EARLY, PRE_KERNEL_1, PRE_KERNEL_2 등 다양하기 때문에 어느 레벨이 언제 실행되는지에 대해서 확실하게 파악하기 위해서다.

기본적으로 Zephyr에서는 부팅 후 커널 초기화를 위해 불리는 함수로 `z_cstart`가 있다.
이 함수 내부에서 커널 초기화 루틴이 진행되게 되는데, 개발자는 sys_init을 통해 해당 함수 내부에서 특정 시점에 호출되는 초기화 루틴을 등록할 수가 있다.

진행 순서는 아래와 같다:

```
1. Initial Stack 로딩 (부트로더)

2. XIP 커널일 경우, ROM에서 RAM으로 데이터 로딩

3. ELF 로더 사용 안할 경우, BSS 섹션을 0으로 세팅

4. z_cstart 함수 호출 (주요 항목만 정리)

    4-1. SYS_INIT_EARLY 루틴 실행

    4-2. arch_kernel_init
        - Architecture-specific kernel initialization hook.
            - <https://docs.zephyrproject.org/latest/hardware/porting/arch.html#c.arch_kernel_init>
        - z_cstart() 내부에서 거의 최상단 위치에서 호출됨
        - x86_32의 32bit 커널 보안 모드나 ARM의 Cortex-M 등의 커널 보안 설정 초기화

    4-3. SYS_INIT_KERNEL_1 레벨 루틴 실행

    4-4. SYS_INIT_KERNEL_2 레벨 루틴 실행

5. main 함수 실행
```
