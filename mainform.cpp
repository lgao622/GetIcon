#include "mainform.h"
#include "ui_mainform.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QPixmap>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileDialog>

int idx = 0;
ICONDIR ico;

MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    memset(&ico, 0, sizeof(ICONDIR));
    ico.idType = 1;
    ico.idCount = 1;
    ico.idEntries[0].wPlanes = 1;
    ico.idEntries[0].dwImageOffset = 22;

    setWindowTitle("");
    setAcceptDrops(true);
    ui->lstIcon->setAlternatingRowColors(true);
    ui->lstIcon->addAction(ui->actionSave);
    ui->lstIcon->setContextMenuPolicy(Qt::ActionsContextMenu);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::on_lstIcon_itemSelectionChanged()
{
    QListWidgetItem* item = ui->lstIcon->currentItem();
    if (item) {
        QIcon ico = item->icon();
        ui->labIcon->setPixmap(ico.pixmap(ico.availableSizes().first().width()));
    }
}

BOOL CALLBACK UpdateIcons(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG lParam)
{
    Ui::MainForm *ui = (Ui::MainForm *)lParam;
    HRSRC hRes = FindResource(hModule, lpszName, lpszType);
    HGLOBAL hResLoaded = LoadResource(hModule, hRes);
    void* pData = LockResource(hResLoaded);
    int nSizeOfIconRes = SizeofResource(hModule,hRes);

    qDebug() << "data:" << pData << ":" << nSizeOfIconRes;
    QByteArray arr;
    ico.idEntries[0].dwBytesInRes = nSizeOfIconRes*4;
    ico.idEntries[0].bWidth = ico.idEntries[0].bHeight = (byte)((char*)pData)[4];

    arr.clear();
    arr.append((char*)&ico, 22);
    arr.append((char*)pData, nSizeOfIconRes);
    QPixmap p;
    p.loadFromData(arr, "ico");
    QString str = QString("%1  (%2x%2)").arg(++idx).arg(p.width());
    QListWidgetItem* item = new QListWidgetItem(str);
    item->setIcon(QIcon(p));
    ui->lstIcon->addItem(item);

    UnlockResource(hResLoaded);
    FreeResource(hResLoaded);

    return TRUE;
}

void MainForm::getIcons(const QString &appName)
{
    idx = 0;
    ui->lstIcon->clear();
    HMODULE hModule = LoadLibraryExA( appName.toLocal8Bit().data(), NULL, LOAD_LIBRARY_AS_DATAFILE );//::LoadLibraryA(appName.toLocal8Bit().data());
    qDebug() << "e:" << EnumResourceNames(hModule, RT_ICON, UpdateIcons, (long)this->ui);
    ::FreeLibrary(hModule);
}

void MainForm::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->urls().size() != 1)
        return;
    QString name = e->mimeData()->urls().first().toString();
    if (name.endsWith(".exe", Qt::CaseInsensitive) || name.endsWith(".dll", Qt::CaseInsensitive))
        e->acceptProposedAction();
}

void MainForm::dropEvent(QDropEvent *e)
{
    QString name = e->mimeData()->urls().first().toString().remove("file:///");
    setWindowTitle(name);
    getIcons(name);
}

void MainForm::on_actionSave_triggered()
{
    QListWidgetItem* item = ui->lstIcon->currentItem();
    if (!item) return;
    QString strFile = QFileDialog::getSaveFileName(0, "保存图标", QString(), "*.png");
    if (!strFile.isEmpty())
        ui->labIcon->pixmap()->save(strFile, ".png");
}
