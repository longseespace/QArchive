#ifndef QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QEventLoop>
#include <QStringList>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QJsonObject>
#include <qarchiveutils_p.hpp>

namespace QArchive {
class DiskExtractorPrivate : public QObject {
    Q_OBJECT
  public:
    DiskExtractorPrivate();
    ~DiskExtractorPrivate();
  public Q_SLOTS:
    void setArchive(QIODevice*);
    void setArchive(const QString&);
    void setBlockSize(int);
    void setCalculateProgress(bool);
    void setOutputDirectory(const QString&);
    void setPassword(const QString&);
    void addFilter(const QString&);
    void addFilter(const QStringList&);
    void clear();

    void getInfo();

    void start();
    void cancel();
    void pause();
    void resume();

  private Q_SLOTS:
    short openArchive();
    short checkOutputDirectory();
    short getTotalEntriesCount();
    short processArchiveInformation();
    short writeData(struct archive_entry*);
    short extract();
  Q_SIGNALS:
    void started();
    void canceled();
    void paused();
    void resumed();
    void finished();
    void error(short);
    void info(QJsonObject);
    void progress(QString, int, int, qint64, qint64);
    void getInfoRequirePassword(int);
    void extractionRequirePassword(int);
  private:
    bool b_PauseRequested = false,
         b_CancelRequested = false,
         b_Paused = false,
         b_Started = false,
         b_Finished = false,
         b_NoProgress = true,
         b_ArchiveOpened = false;
    int n_PasswordTriedCountGetInfo = 0,
        n_PasswordTriedCountExtract = 0,
        n_ProcessedEntries = 0,
        n_TotalEntries = -1,
        n_BlockSize = 10240,
        n_Flags = 0;
    qint64 n_BytesProcessed = 0,
           n_BytesTotal = 0;

    QString m_OutputDirectory,
            m_Password,
            m_ArchivePath;
    QIODevice *m_Archive = nullptr;
    QSharedPointer<struct archive> m_ArchiveRead;
    QSharedPointer<struct archive> m_ArchiveWrite;
    QScopedPointer<QStringList> m_ExtractFilters;
    QScopedPointer<QJsonObject> m_Info;
};
}
#endif // QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
