#include "principal.h"
#include "ui_principal.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPoint>

#define DEFAULT_ANCHO 3

// Constructor
Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)-- Graficos por computadora que tienen diferentes formatos. Formato png es ARGB32
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing); // -> Graficos por computadora, es algo que mejora en los dispositivos lcd
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
}

// Destructor
Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal para poder tener la pantalla donde se va a dibujar
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Aceptar el evento
    event->accept();
}

// Evento se dispara cuando presionas con el mouse
void Principal::mousePressEvent(QMouseEvent *event)
{
    // Levanta la bandera (Para que se pueda dibujar)
    mPuedeDibujar = true;
    // Capturar la posición (Punto x,y) del mouse
    mInicial = event->pos();
    // Acepta el evento
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        // Acepta el evento
        event->accept();
        // Salir del método
        return;
    }
    // Capturar el punto o posición a donde se mueve el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea con el Painter (Pintor) Principal
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    // Mostrar el número de líneas en la barra de estado
    ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
    // Actualizar la interfaz -> Se invoca al método PaintEvent (Repintar con paintEvent)
    // Vuelve a dibujar la imagen para ver los cambios que surgen
    update();
    // Actualizar el punto inicial
    mInicial = mFinal;
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    mPuedeDibujar = false;
    // Aceptar el vento
    event->accept();

}


void Principal::on_actionAncho_triggered()
{
    // Spin box que permite ingresar numeros enteros
    // QInputDialog proporciona una simple ventana de diálogo
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 20);
}

void Principal::on_actionSalir_triggered()
{
    if(!mImagen->isNull()){
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this,
                    "Salir","Desea guardar el archivo",
                    QMessageBox::Save | QMessageBox::Cancel);

        if(reply == QMessageBox::Save ) {
            on_actionGuardar_triggered();
        } else{
            this->close();
        }
    }else{
        this->close();
    }

}

void Principal::on_actionColor_triggered()
{
    // QColorDialog Es un cuadro de diálogo de un widget selector de color
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del Pincel");
}

void Principal::on_actionNuevo_triggered()
{
    if(!mImagen->isNull()){
        QMessageBox::StandardButton reply = QMessageBox::question(
                     this,
                     "Nuevo Archivo","Desea guardar el archivo",
                    QMessageBox::Save | QMessageBox::Cancel);

        if(reply == QMessageBox::Save ) {
            on_actionGuardar_triggered();
        } else{
            mImagen->fill(Qt::white);
            mNumLineas = 0;
            update();
        }
    }else{
        mImagen->fill(Qt::white);
        mNumLineas = 0;
        update();
    }

}

void Principal::on_actionLineas_triggered()
{
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea con el Painter (Pintor) Principal
    QLine linea(mInicial.x(),mInicial.ry(),mFinal.x(),mFinal.ry());
    mPainter->drawLine(linea);
    // Actualizar la interfaz -> Se invoca al método PaintEvent (Repintar con paintEvent)
    // Vuelve a dibujar la imagen para ver los cambios que surgen
    update();

}

void Principal::on_actionRect_nculos_triggered()
{
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea con el Painter (Pintor) Principal
    mPainter->setPen(pincel);
    mPainter->drawRect(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());
    // Actualizar la interfaz -> Se invoca al método PaintEvent (Repintar con paintEvent)
    // Vuelve a dibujar la imagen para ver los cambios que surgen
    update();
}

void Principal::on_actionCircunferencias_triggered()
{
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea con el Painter (Pintor) Principal
    mPainter->setPen(pincel);
    mPainter->drawEllipse(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());
    // Actualizar la interfaz -> Se invoca al método PaintEvent (Repintar con paintEvent)
    // Vuelve a dibujar la imagen para ver los cambios que surgen
    update();
}

void Principal::on_actionGuardar_triggered()
{
    // Abrir un cuadro de diálogo para obtener el nombre del Archivo
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes(*.png)"); // Se conoce como el filtro solo muestra png
    // Validar que el Nombre del Archivo no sea vacío
    if ( !nombreArchivo.isEmpty() ){
        // Guardar la Imagen con un método
        if (mImagen->save(nombreArchivo))
            // Si todo va bien muestra un Mensaje de Información
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
    }else{
        // Si hay algún error, muestro advertencia
        QMessageBox::warning(this,
                             "Guardar imagen",
                             "No se pudo almacenar la imagen.");
    }
}




void Principal::on_actionAcerca_de_triggered()
{
    // Crear un objeto del cuadro de diálogo
    Acerca *dialog = new Acerca(this);
    // Enviar datos a la otra ventana
    dialog->setVersion(VERSION);
    // Mostrar la venta en modo MODAL
    dialog->exec();
    // Luego de cerrar la ventana, se acceden a los datos de la misma
    qDebug() << dialog->valor();
}

