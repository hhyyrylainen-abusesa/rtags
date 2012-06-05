#ifndef INDEXER_H
#define INDEXER_H

#include <QtCore>
#include <AddMessage.h>
#include "Rdm.h"
#include "CursorInfo.h"
#include <clang-c/Index.h>

class IndexerJob;
class Indexer : public QObject
{
    Q_OBJECT;
public:

    Indexer(const QByteArray &path, QObject* parent = 0);
    ~Indexer();

    enum IndexType {
        DirtyPch = 1,
        Dirty = 2,
        Makefile = 3 // these are used as QThreadPool priorites
    };
    int index(const Path &input, const QList<QByteArray> &arguments, IndexType type);

    void setPchDependencies(const Path &pchHeader, const QSet<quint32> &deps);
    void addDependencies(const DependencyHash &hash);
    QSet<quint32> pchDependencies(const Path &pchHeader) const;
    QHash<QByteArray, Location> pchUSRHash(const QList<Path> &pchFiles) const;
    void setPchUSRHash(const Path &pch, const PchUSRHash &astHash);
    Path path() const { return mPath; }
    void abort();
    bool visitFile(quint32 fileId, const Path &p);
signals:
    void indexingDone(int id);
    void jobsComplete();
private slots:
    void onJobComplete(int id, const Path &input, bool isPch, const QByteArray &msg);
    void onDirectoryChanged(const QString &path);
private:
    void dirty(const QSet<quint32> &files);
    void commitDependencies(const DependencyHash &deps, bool sync);
    void initDB();
    bool needsToWaitForPch(IndexerJob *job) const;
    void startJob(int id, IndexerJob *job);

    mutable QReadWriteLock mPchUSRHashLock;
    QHash<Path, PchUSRHash> mPchUSRHashes;

    QMutex mVisitedFilesMutex;
    QSet<quint32> mVisitedFiles;

    mutable QReadWriteLock mPchDependenciesLock;
    QHash<Path, QSet<quint32> > mPchDependencies;
    int mJobCounter;

    QMutex mMutex;
    QSet<Path> mIndexing;

    QByteArray mPath;
    QHash<int, IndexerJob*> mJobs, mWaitingForPCH;

    bool mTimerRunning;
    QElapsedTimer mTimer;

    QFileSystemWatcher mWatcher;
    DependencyHash mDependencies;
    QMutex mWatchedMutex;
    WatchedHash mWatched;
};

inline bool Indexer::visitFile(quint32 fileId, const Path &path)
{
    QMutexLocker lock(&mVisitedFilesMutex);
    if (mVisitedFiles.contains(fileId)) {
        return false;
    }
    mVisitedFiles.insert(fileId);
    return true;
}
#endif
