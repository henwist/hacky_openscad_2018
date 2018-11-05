/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_View_t {
    QByteArrayData data[11];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_View_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_View_t qt_meta_stringdata_View = {
    {
QT_MOC_LITERAL(0, 0, 4), // "View"
QT_MOC_LITERAL(1, 5, 15), // "doAnimateUpdate"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 6), // "ZoomIn"
QT_MOC_LITERAL(4, 29, 7), // "ZoomOut"
QT_MOC_LITERAL(5, 37, 9), // "showFaces"
QT_MOC_LITERAL(6, 47, 9), // "showEdges"
QT_MOC_LITERAL(7, 57, 8), // "showAxes"
QT_MOC_LITERAL(8, 66, 14), // "showCrosshairs"
QT_MOC_LITERAL(9, 81, 9), // "orthoMode"
QT_MOC_LITERAL(10, 91, 21) // "showScaleProportional"

    },
    "View\0doAnimateUpdate\0\0ZoomIn\0ZoomOut\0"
    "showFaces\0showEdges\0showAxes\0"
    "showCrosshairs\0orthoMode\0showScaleProportional"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_View[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       6,   32, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       5, QMetaType::Bool, 0x00095103,
       6, QMetaType::Bool, 0x00095103,
       7, QMetaType::Bool, 0x00095103,
       8, QMetaType::Bool, 0x00095103,
       9, QMetaType::Bool, 0x00095103,
      10, QMetaType::Double, 0x00095103,

       0        // eod
};

void View::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        View *_t = static_cast<View *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doAnimateUpdate(); break;
        case 1: _t->ZoomIn(); break;
        case 2: _t->ZoomOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (View::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&View::doAnimateUpdate)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        View *_t = static_cast<View *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->showFaces(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->showEdges(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->showAxes(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->showCrosshairs(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->orthoMode(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->showScaleProportional(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        View *_t = static_cast<View *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setShowFaces(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setShowEdges(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setShowAxes(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setShowCrosshairs(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setOrthoMode(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setShowScaleProportional(*reinterpret_cast< double*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject View::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_View.data,
      qt_meta_data_View,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *View::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *View::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_View.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IView"))
        return static_cast< IView*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int View::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void View::doAnimateUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
