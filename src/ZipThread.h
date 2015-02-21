#ifndef ZIPTHREAD_H_
#define ZIPTHREAD_H_

#include <QObject>
#include <QRunnable>
#include <QString>

namespace canadainc {

class ZipThread : public QObject, public QRunnable
{
	Q_OBJECT

	QString m_fileName;
	bool m_extractHere;
	bool m_cleanup;
	const char* m_password;
	QString m_destinationFolder;

signals:
	void done(bool success, QString const& error=QString());
	void progress(int current, int entries);
	void deflationProgress(qint64 current, qint64 total);

public:
	/**
	 * @param cleanup Delete the archive after extraction is complete.
	 */
	ZipThread(QString const& fileName, const char* password=NULL, bool extractHere=true, bool cleanup=true);
	virtual ~ZipThread();

	void run();
	void setDestinationFolder(QString const& folder);
};

} /* namespace canadainc */
#endif /* ZIPTHREAD_H_ */
