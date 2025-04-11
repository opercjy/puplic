# CMake 최소 요구 버전 설정 (Geant4 11.3 에 맞게)
cmake_minimum_required(VERSION 3.16)

# 프로젝트 이름 설정
project(NuclearMedicine_DNA_OR_TUMOR)

# C++ 표준 설정 (Geant4 11.3 은 C++17 권장)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Geant4 패키지 찾기 및 설정
# Geant4 설치 경로를 시스템 환경 변수 또는 CMake 변수로 지정해야 할 수 있음
# 예: cmake .. -DGeant4_DIR=/path/to/geant4/install/lib64/Geant4-11.3.0
find_package(Geant4 REQUIRED)

# Geant4 CMake 모듈 포함
include(${Geant4_USE_FILE})

# 전역 Geant4 인터페이스 라이브러리 포함 (헤더 등)
include_directories(${Geant4_INCLUDE_DIRS})

# 실행 파일 이름 정의
add_executable(nuclear_med_dna main.cc)

# 소스 파일 그룹화 (선택 사항이지만 관리 용이)
file(GLOB sources "src/*.cc")
target_sources(nuclear_med_dna PRIVATE ${sources})

# include 디렉토리 추가
target_include_directories(nuclear_med_dna PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Geant4 라이브러리 링크
target_link_libraries(nuclear_med_dna ${Geant4_LIBRARIES})

# Geant4 데이터셋 복사 (필요한 경우)
# G4데이터셋 환경변수가 설정되어 있어야 함
# add_custom_command(TARGET nuclear_med_dna POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${PROJECT_SOURCE_DIR}/data $<TARGET_FILE_DIR:nuclear_med_dna>/data )

# 설치 규칙 (선택 사항)
# install(TARGETS nuclear_med_dna DESTINATION bin)