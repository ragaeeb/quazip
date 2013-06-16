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

signals:
	void done(bool success, QString const& error=QString());
	void progress(int current, int entries);

public:
	/**
	 * @param cleanup Delete the archive after extraction is complete.
	 */
	ZipThread(QString const& fileName, bool extractHere=true, bool cleanup=true);
	virtual ~ZipThread();

	void run();
};

} /* namespace canadainc */
#endif /* ZIPTHREAD_H_ */
