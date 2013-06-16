#include "ZipThread.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "quazip.h"
#include "quazipfile.h"

namespace canadainc {

ZipThread::ZipThread(QString const& fileName, bool extractHere, bool cleanup) :
		m_fileName(fileName), m_extractHere(extractHere), m_cleanup(cleanup)
{
}


void ZipThread::run()
{
	QFile file(m_fileName);

	if ( !file.exists() )
	{
		emit done( false, tr("Archive could not be found.") );
		return;
	}

	bool result = true;
	QString resultComment;
	QuaZip zip(m_fileName);

	QString dest = QDir::homePath();

	if (!m_extractHere) {
		QString bookname = m_fileName.split("/").last().split(".").first();
		dest += "/" + bookname;

		QDir dir(dest);
		if ( !dir.exists() ) {
			dir.mkpath("."); // create destination folder
		}
	}

	bool opened = zip.open(QuaZip::mdUnzip);

	if (!opened) {
		emit done( false, tr("Could not open archive for decompression.") );
		return;
	}

	QuaZipFile currentFile(&zip);
	int entries = zip.getEntriesCount();
	int current = 0;

	for ( bool more = zip.goToFirstFile(); more; more = zip.goToNextFile() )
	{
		++current;
		// if the entry is a path ignore it. Path existence is ensured separately.
		if ( zip.getCurrentFileName().split("/").last() == "" ) {
			continue;
		}

		QString outfilename = dest + "/" + zip.getCurrentFileName();
		QFile outputFile(outfilename);

		if ( !QDir().mkpath( QFileInfo(outfilename).absolutePath() ) ) {
			result = false;
			resultComment = QString("Creating output path failed for: %1").arg(outfilename);
			break;
		}

		if ( !outputFile.open(QFile::WriteOnly) ) {
			result = false;
			resultComment = QString("Creating output file failed: %1").arg(outfilename);
			break;
		}

		currentFile.open(QIODevice::ReadOnly);
		outputFile.write( currentFile.readAll() );

		if ( currentFile.getZipError() != UNZ_OK ) {
			result = false;
			resultComment = QString("Zip error: %1, on file %2").arg( currentFile.getZipError() ).arg( currentFile.getFileName() );
			break;
		}

		currentFile.close();
		outputFile.close();

		emit progress(current, entries);
	}

	file.close();

	if (m_cleanup) {
		file.remove();
	}

	emit done(result, resultComment);
}


ZipThread::~ZipThread()
{
}

} /* namespace canadainc */
