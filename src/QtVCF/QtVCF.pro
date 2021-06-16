NAME         = QtVCF
TARGET       = $${NAME}

QT           = core
QT          += gui
QT          += network
QT          += sql
QT          += script
QT          += positioning
QT          += widgets
QT          += opengl
QT          += printsupport
QT          += multimedia
QT          += multimediawidgets
QT          += opengl
QT          += QtCUDA
QT          += QtOpenCV
QT          += Essentials
QT          += QtCalendar
QT          += QtGMP
QT          += QtGSL
QT          += QtAlgebra
QT          += QtDiscrete
QT          += QtFFT
QT          += Mathematics
QT          += QtFuzzy
QT          += QtFLP
QT          += QtFoundation
QT          += QtGeometry
QT          += QtGadgets
QT          += QtComponents
QT          += QtManagers

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtvcf.h

include ($${PWD}/GraphicsView/GraphicsView.pri)
include ($${PWD}/Items/Items.pri)

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)

win32 {

OPENCVVER    = 310

  CONFIG(debug,debug|release) {
    LIBS    += -lfreetyped
    LIBS    += -lopencv_world$${OPENCVVER}d
  } else {
    LIBS    += -lfreetype
    LIBS    += -lopencv_world$${OPENCVVER}
  }

LIBS        += -lglu32
LIBS        += -lopengl32
}
