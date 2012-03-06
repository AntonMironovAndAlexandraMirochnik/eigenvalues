FORMS += \
    ui/main_window.ui \
    ui/comparation_widget.ui \
    ui/eigenvalues_widget.ui

HEADERS += \
    model/matrix/matrix_private.h \
    model/matrix/matrix_globals.h \
    model/matrix/matrix_exceptions.h \
    model/matrix/matrix.h \
    model/matrix/editable_matrix.h \
    model/matrix_wrapper/matrix_table_view.h \
    model/matrix_wrapper/matrix_model.h \
    controller/main_controller.h \
    controller/eigenvalues_algorithm_controller.h \
    controller/qr_algorithm_controller.h \
    controller/jacobi_algorithm_controller.h \
    model/tasks/eigenvalues_task.h \
    controller/comparation_controller.h \
    model/matrix_wrapper/matrix_table_item_delegate.h \
    constants.h

SOURCES += \
    model/matrix/matrix_solve_linear.cpp \
    model/matrix/matrix_private.cpp \
    model/matrix/matrix_exceptions.cpp \
    model/matrix/matrix.cpp \
    model/matrix/editable_matrix.cpp \
    model/matrix_wrapper/matrix_table_view.cpp \
    model/matrix_wrapper/matrix_model.cpp \
    controller/main_controller.cpp \
    main.cpp \
    controller/eigenvalues_algorithm_controller.cpp \
    controller/qr_algorithm_controller.cpp \
    controller/jacobi_algorithm_controller.cpp \
    model/tasks/eigenvalues_task.cpp \
    model/tasks/qr_task.cpp \
    model/tasks/jacobi_task.cpp \
    controller/comparation_controller.cpp \
    model/matrix_wrapper/matrix_table_item_delegate.cpp

VERSION = 0.0.3.0

DEFINES += "APPLICATION_VERSION=QString(\\\"$${VERSION}\\\")" \
	    "nullptr=0"

RC_FILE = resource/win.rc

RESOURCES += resource/resource.qrc

TRANSLATIONS += \
    resource/en.ts \
    resource/ru.ts
