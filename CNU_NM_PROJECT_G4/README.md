# Geant4 핵의학 DNA 손상 시뮬레이션 템플릿 (빌드 검증용)

# 정통 핵/입자 물리 관점에서 계산과학 연구를 보여주기 위함.

## 개요

이 프로젝트는 Geant4 11.3 버전을 사용하여 핵의학 시나리오를 시뮬레이션하기 위한 기본 코드 템플릿입니다. 특히, 종양 세포 내 DNA 분자에 대한 방사선 의약품(진단용 감마선 vs 치료용 하전 입자)의 영향을 비교하는 것을 목표로 합니다.

**!!! 중요 공지 !!!**

**이 코드는 완전한 시뮬레이션 코드가 아닌, 기본적인 구조를 제공하는 템플릿입니다. 빌드 가능성을 확인하는 목적으로만 제공되며, 포함된 물리 모델, 기하학적 구조, 데이터 수집 방식 등에 대한 정확성 및 유효성은 검증되지 않았습니다. 실제 연구나 분석에 사용하기 전에 사용자는 반드시 코드를 자신의 연구 목적에 맞게 수정, 확장하고 철저히 검증해야 합니다.**

## 목표 시나리오 (템플릿 기준)

* **입자 소스:** 1 MeV 감마선, 1 MeV 전자, 또는 1 MeV 양성자 (매크로 또는 코드로 선택 가능)
* **타겟:** 물(Water)로 근사된 종양 세포 클러스터 내부에 배치된 2~4개의 단순화된 DNA 모델 (실린더 또는 구 형태, 물로 근사)
* **물리 모델:** **GEANT4-DNA** 물리 리스트 (예: `G4EmDNAPhysics_opt4`) 사용. 미세 구조에서의 저에너지 상호작용 및 DNA 손상 연구에 필수적입니다. (Shielding 물리 리스트는 이 목적에 부적합합니다.)
* 또한 시공간적에 아주 작은 영역에 입사된 고에너지 고선량 중입자의 원자, 분자 기반 선량과 그 생물학적 연구에는 다른 물리 상호작용 선택함.  
* **데이터 수집:** DNA 볼륨 내 에너지 축적(Energy Deposition)을 기록하기 위한 기본 구조 (SteppingAction, EventAction, RunAction, Accumulable 활용 예시 포함).

학문후속세대가 익숙해지면 충돌기, 검출기, 천체 연구 관련 익숙한 방법론들을 구현.

## 요구 사항

* **Geant4:** 버전 11.3 이상 (GEANT4-DNA 옵션(`GEANT4_USE_DNA=ON`)을 활성화하여 빌드해야 함)
* **CMake:** 버전 3.16 이상
* **C++ 컴파일러:** C++17 표준 지원 (GCC, Clang 등)
* **운영체제:** Rocky Linux (테스트 환경). 다른 Linux 배포판에서도 호환될 가능성이 높습니다.
* **Geant4 데이터 파일:** 환경 변수 (예: `G4LEDATA`, `G4ENSDFSTATEDATA` 등)가 올바르게 설정되어 있어야 합니다.

## 프로젝트 구조

your_project_name/
├── CMakeLists.txt           # 빌드 스크립트
├── include/                 # 헤더 파일 디렉토리
│   ├── ActionInitialization.hh
│   ├── DetectorConstruction.hh
│   ├── EventAction.hh
│   ├── PhysicsList.hh
│   ├── PrimaryGeneratorAction.hh
│   ├── RunAction.hh
│   └── SteppingAction.hh
├── src/                     # 소스 파일 디렉토리
│   ├── ActionInitialization.cc
│   ├── DetectorConstruction.cc
│   ├── EventAction.cc
│   ├── PhysicsList.cc
│   ├── PrimaryGeneratorAction.cc
│   ├── RunAction.cc
│   └── SteppingAction.cc
├── main.cc                  # 메인 어플리케이션 파일
└── README.md                # 이 파일

## 주요 특징 (템플릿 수준)

* 표준 Geant4 어플리케이션 구조 사용 (`main`, `DetectorConstruction`, `PhysicsList`, `ActionInitialization` 등).
* 간단한 월드, 세포 클러스터, DNA 기하학적 구조 정의 예시 (`DetectorConstruction`).
* **GEANT4-DNA** 물리 리스트 등록 (`PhysicsList`).
* `G4ParticleGun`을 이용한 기본 입자 소스 설정 (`PrimaryGeneratorAction`).
* 에너지 침적 감지를 위한 Sensitive Detector 및 Primitive Scorer 설정 예시 (`DetectorConstruction::ConstructSDandField`).
* 스텝/이벤트/런 단위 데이터 처리를 위한 Action 클래스 구조 (`SteppingAction`, `EventAction`, `RunAction`).
* 멀티스레딩 환경에서의 데이터 집계를 위한 `G4Accumulable` 사용 예시 (`RunAction`).
* (주석 처리됨) `G4AnalysisManager`를 이용한 결과 파일(예: ROOT Ntuple) 출력 기본 구조 포함.

## 빌드 방법

1.  프로젝트 루트 디렉토리에서 빌드 디렉토리를 생성하고 이동합니다.
    ```bash
    mkdir build
    cd build
    ```
2.  CMake를 실행하여 빌드 환경을 구성합니다. Geant4 설치 경로를 `Geant4_DIR` 환경 변수 또는 CMake 옵션으로 명시해야 할 수 있습니다.
    ```bash
    # Geant4 설치 경로를 직접 지정하는 경우
    cmake .. -DGeant4_DIR=/path/to/your/geant4/install/lib64/Geant4-11.3.0

    # Geant4 환경 변수가 시스템에 설정된 경우
    # cmake ..
    ```
3.  `make` 명령어를 사용하여 코드를 컴파일합니다. (`-jN` 옵션으로 병렬 빌드 가능, N은 코어 수)
    ```bash
    make -j4
    ```
    빌드가 성공하면 `build` 디렉토리 내에 `nuclear_med_dna` 실행 파일이 생성됩니다.

## 실행 방법

* **GUI 모드:**
    ```bash
    ./nuclear_med_dna
    ```
    Geant4 UI 창이 나타나면, 매크로 파일(예: `init.mac`, `run.mac` 등)을 실행하거나 UI 명령어를 입력하여 시뮬레이션을 제어할 수 있습니다.

* **Batch 모드:** (실행할 명령어가 담긴 매크로 파일 `run.mac`이 있다고 가정)
    ```bash
    ./nuclear_med_dna run.mac
    ```

    **매크로 파일 예시 (`run.mac`):**
    ```geant4
    # 입자 종류 설정 (gamma, e-, proton 중 선택)
    /gun/particle e-
    # 에너지 설정
    /gun/energy 1 MeV
    # 시뮬레이션 실행 (1000 이벤트)
    /run/beamOn 1000
    ```

## 향후 작업 / 사용자 정의 필요 사항

이 템플릿을 실제 연구에 활용하기 위해서는 다음 사항들에 대한 사용자 정의 및 검증이 필수적입니다.

* **정밀한 기하학적 모델링:**
    * 세포 및 세포 내 소기관 (핵 등)의 실제적인 크기와 형태 반영.
    * DNA의 보다 현실적인 모델링 (단순 실린더/구 이상, 필요시 PDB 파일 활용 등).
    * DNA의 정확한 위치 및 방향 설정.
* **물리 프로세스 검토/선택:** 사용 중인 GEANT4-DNA 옵션(`opt6`)이 연구 목적에 적합한지 확인하고, 필요시 다른 옵션(`opt0`, `opt4` 등) 또는 추가적인 물리 프로세스(예: 화학 단계 시뮬레이션)를 고려합니다.
* **입자 소스 상세 설정:** 실제 방사성 의약품의 방출 스펙트럼, 위치 분포 등을 반영합니다.
* **데이터 분석 로직 구현:**
    * `SteppingAction`, `EventAction`, `RunAction` 내에서 필요한 데이터를 정밀하게 추출하고 저장하는 로직 구현.
    * `G4AnalysisManager` 등을 활용하여 원하는 형태의 결과(히스토그램, Ntuple 등)를 생성하고 분석합니다.
* **시뮬레이션 파라미터 최적화:** Cut 설정, 스텝 제한 등을 조절하여 결과의 정확성과 계산 시간 사이의 균형을 맞춥니다.
* **결과 검증:** 다른 시뮬레이션 코드, 실험 데이터 등과 비교하여 시뮬레이션 결과의 타당성을 검증합니다.

## 참고

* 이 템플릿은 GEANT4-DNA 물리 리스트를 사용하도록 구성되었습니다. 이는 나노미터 스케일의 생물학적 매질 내 상호작용을 상세히 모델링하기 위함입니다. 고에너지 입자의 차폐(Shielding) 계산 등 다른 목적에는 적합하지 않습니다.
