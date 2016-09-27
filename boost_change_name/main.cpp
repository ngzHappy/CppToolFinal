#include <thread>
#include <memory>
#include <atomic>
#include <QtCore>
#include <QtConcurrent>
#include <iostream>

namespace main_quit_code {
constexpr const int ok=0;
enum ErrorCode :int {
    argsError=-2,
};

}

class CommandLineParser final :public QCommandLineParser {
public:
    CommandLineParser();
    ~CommandLineParser();
};

inline QString operator""_qs(const char * arg,std::size_t N) {
    return QString::fromUtf8(arg,static_cast<int>(N));
}

class DutyFiles {
public:

    enum Type :int { hpp,cpp,c,h,unknow };

    class Item {
    public:
        Type type;
        QString fileName;
    };

    QLinkedList<Item> files;
};

DutyFiles findAllFiles(const QString &);
void changeAllFiles(const DutyFiles *);

int main(int argc,char *argv[]) try {
    QCoreApplication app(argc,argv);

    QString varInputDirPath;

    {
        CommandLineParser varParser;
        varParser.process(app);

        varInputDirPath=varParser.value(u8R"(input)"_qs);
    }

    if (varInputDirPath.isEmpty()) {
        throw main_quit_code::ErrorCode::argsError;
    }

    auto varDutyFiles=findAllFiles(varInputDirPath);

    if (varDutyFiles.files.isEmpty()) {
        return 0;
    }

    changeAllFiles(&varDutyFiles);

    return main_quit_code::ok;
}
catch (const main_quit_code::ErrorCode&e) {
    switch (e) {
        case main_quit_code::ErrorCode::argsError: {qDebug()<<"args error please input -h for detail"; }break;
    }
    return e;
}

CommandLineParser::CommandLineParser() {
    setApplicationDescription(u8R"(boost_change_name)"_qs);
    addHelpOption();
    addVersionOption();

    {
        QCommandLineOption var(
                    u8R"(input)"_qs/*name*/,
                    u8R"(input dir)"_qs/*description*/,
                    u8R"(input)"_qs/*valueName*/);
        addOption(std::move(var));
    }

}

CommandLineParser::~CommandLineParser() {

}

static inline auto is_hpp(const QString & _arg) {
    return QString::compare(_arg,QLatin1String("hpp",3),Qt::CaseInsensitive)==0;
}

static inline auto is_h(const QString & _arg) {
    return QString::compare(_arg,QLatin1String("h",1),Qt::CaseInsensitive)==0;
}

static inline auto is_cpp(const QString & _arg) {
    return QString::compare(_arg,QLatin1String("cpp",3),Qt::CaseInsensitive)==0;
}

static inline auto is_c(const QString & _arg) {
    return QString::compare(_arg,QLatin1String("c",1),Qt::CaseInsensitive)==0;
}

DutyFiles findAllFiles(const QString &arg) {

    QDirIterator varDirIterator(
        arg,
        QDir::Files|QDir::NoSymLinks,
        QDirIterator::Subdirectories);

    DutyFiles varAns;

    while (varDirIterator.hasNext()) {
        varDirIterator.next();
        auto varFileInfo=varDirIterator.fileInfo();
        auto varSuffix=varFileInfo.suffix();
        DutyFiles::Item varItem;

        if (is_hpp(varSuffix)) { varItem.type=DutyFiles::hpp; }
        else if (is_h(varSuffix)) { varItem.type=DutyFiles::h; }
        else if (is_cpp(varSuffix)) { varItem.type=DutyFiles::cpp; }
        else if (is_c(varSuffix)) { varItem.type=DutyFiles::c; }
        else { varItem.type=DutyFiles::unknow; }

        varItem.fileName=varFileInfo.absoluteFilePath();

        varAns.files.push_back(std::move(varItem));
    }

    return std::move(varAns);
}

namespace {
namespace __private {
namespace __changeAllFiles {

namespace bytearray {
const static QByteArray qt_boost_(u8R"(Qt/boost/)");
const static QByteArray boost_(u8R"(boost/)");
const static QByteArray _rn_("\r\n");
}

const static QString qt_boost_(QString::fromUtf8(bytearray::qt_boost_));
const static QString boost_(QString::fromUtf8(bytearray::boost_));
const static QRegularExpression r_qt_boost_(u8R"(\bboost/)"_qs);

void do_1(const QString & fileName) {

    const static auto write_codec=QTextCodec::codecForName("utf-8");

    QByteArray varFile;

    {/*read the file*/
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            varFile=file.readAll();
        }
        else { return; }

        {/*remove the last endl*/
            const auto varFileSize=varFile.size();
            if (varFileSize<1) { return; }
            if (varFile.endsWith(bytearray::_rn_)) { varFile.resize(varFileSize-2); }
            else if (varFile.endsWith('\n')) { varFile.resize(varFileSize-1); }
            else if (varFile.endsWith('\r')) { varFile.resize(varFileSize-1); }
        }

        if (varFile.isEmpty()) { return; }
        if (varFile.indexOf(bytearray::qt_boost_)>=0) { /*we have done the file*/return; }
        //if (varFile.indexOf(bytearray::boost_)<0) { /*the file no need to done*/return; }
        /*force file to close*/
        file.close();
    }

    {/*write the file*/
        QTextStream readStream(&varFile,QIODevice::ReadOnly);
        QFile varOutFile(fileName);

        if (false==varOutFile.open(QIODevice::WriteOnly)) {
            return;
        }

        QTextStream writeStream(&varOutFile);
        writeStream.setAutoDetectUnicode(true);
        writeStream.setCodec(write_codec);
        writeStream<<bom;

        while (readStream.atEnd()==false) {
            QString varLine=readStream.readLine();
            /***************************************/
            varLine.replace(r_qt_boost_,qt_boost_);
            /***************************************/
            writeStream<<varLine<<endl;
        }
    }

}

}/**/
}/**/
}/**/

void changeAllFiles(const DutyFiles * arg) {

    const auto &varFiles=arg->files;
    std::atomic<bool> varIsFinished=false;

    {
        std::thread varThread([&]() mutable {

            class ThreadLock {
                std::atomic<bool>*const _m_IsFinished;
            public:
                ThreadLock(std::atomic<bool>*arg):_m_IsFinished(arg) { arg->store(false); }
                ~ThreadLock() { _m_IsFinished->store(true); }
            };

            ThreadLock varThreadLock{&varIsFinished};

            /*block here after finished*/
            QtConcurrent::blockingMap(varFiles.cbegin(),varFiles.cend(),
                [](const DutyFiles::Item&i) {
                try {
                    if (i.type==DutyFiles::unknow) { return; }
                    __private::__changeAllFiles::do_1(i.fileName);
                }
                catch (...) {
                    qDebug()<<"unknow exception"<<__LINE__<<__func__<<__FILE__;
                }
            });

        }
        );
        varThread.detach();
    }

    QElapsedTimer varElapsedTimer;
    varElapsedTimer.start();
    std::cout<<"start waiting:"<<std::endl;
    /*wait for finished*/
    while (varIsFinished.load()==false) {
        std::cout<<"."<<std::flush;
    }
    std::cout<<std::endl<<"finished:"
        <<(double(varElapsedTimer.nsecsElapsed())/1000000000.0)<<std::endl;

}


