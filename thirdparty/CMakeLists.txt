if (${MVVM_BUILD_TESTS})
    add_subdirectory(gtest)  
endif()

if (NOT ${MVVM_USE_SYSTEM_QCUSTOMPLOT})
    add_subdirectory(qcustomplot)
endif()
