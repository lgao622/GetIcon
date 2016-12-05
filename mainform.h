#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <qt_windows.h>

namespace Ui {
class MainForm;
}

#pragma pack(push) //保存对齐状态
#pragma pack(1)
typedef struct
{
    BYTE        bWidth;                 // Width, in pixels, of the image
    BYTE        bHeight;                // Height, in pixels, of the image
    BYTE        bColorCount;         // Number of colors in image (0 if >=8bpp)
    BYTE        bReserved;            // Reserved ( must be 0)
    WORD        wPlanes;              // Color Planes
    WORD        wBitCount;           // Bits per pixel
    DWORD       dwBytesInRes;    // How many bytes in this resource?
    DWORD       dwImageOffset;  // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;
#pragma pack(pop)

#pragma pack(push) //保存对齐状态
#pragma pack(1)
typedef struct
{
    WORD           idReserved;       // Reserved (must be 0)
    WORD           idType;              // Resource Type (1 for icons)
    WORD           idCount;            // How many images?
    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;
#pragma pack(pop)

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

private slots:
    void on_lstIcon_itemSelectionChanged();
    void on_actionSave_triggered();

private:
    void getIcons(const QString&appName);

    void dragEnterEvent(QDragEnterEvent*e);
    void dropEvent(QDropEvent*e);

private:
    Ui::MainForm *ui;
};

#endif // MAINFORM_H
