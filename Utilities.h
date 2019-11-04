// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,01.11.2019</created>
/// <changed>ʆϒʅ,03.11.2019</changed>
// ********************************************************************************

#ifndef UTILITIES_H
#define UTILITIES_H


// special expansion to exception container
class theException : public std::exception
{
private:
  const char* expected;
public:
  theException ( void );
  void set ( const char* );
  const char* what ( void ) const throw();
};


// log severity
enum logType { info = 0, debug, warning, error };


// log container structure
struct LogEntity
{
  unsigned int id;
  std::wstring arrivedAt;
  logType type;
  std::thread::id threadId;
  std::wstring threadName;
  std::wstring message;
  LogEntity ( void );
  LogEntity operator=( LogEntity& );
};


// file stream policy
class toFile
{
private:
  std::ofstream fileStream;
  bool ready;
public:
  toFile ( void );
  ~toFile ( void );
  const bool& state ( void );
  bool write ( const std::wstring& );
};


// Todo other stream policy
//class toStream {};


// logging engine
template<class tType>
class Logger
{
private:
  LogEntity theLog;
  std::wstring theLogRawStr;
  std::list<std::wstring> buffer; // buffer list container
  tType filePolicy; // output stream policy
  std::timed_mutex writeGuard; // write guard
  std::thread commit; // write engine thread
  // lock-free atomic flag (checking the running state) (standard initialization):
  std::atomic_flag operating { ATOMIC_FLAG_INIT };

  static unsigned int counter;
public:
  Logger ( void );
  ~Logger ( void );
  void push ( const logType&,
              const std::thread::id&,
              const std::wstring&,
              const std::wstring& );
  const LogEntity& getLog ( void );
  const std::wstring& getLogRawStr ( void );

  template<class tType>
  friend void loggerEngine ( Logger<tType>* ); // write engine
};
void LoggerClassLinker ( void ); // don't call this function: solution for linker error, when using templates.


// configurations container
struct ConfigsContainer
{
  unsigned int Width;
  unsigned int Height;
  bool fullscreen;
};


// application configurations class
class Configurations
{
private:
  std::wstring pathToMyDocuments;
  std::wstring pathToSettings;
  bool valid;
  ConfigsContainer defaults;
  ConfigsContainer currents;
public:
  Configurations ( void );
  const bool& isValid ( void );
  const ConfigsContainer& getDefaults ( void );
  const ConfigsContainer& getSettings ( void );
  void apply ( void ); // test
  const bool apply ( const ConfigsContainer& ); // apply method
};// application configurations container


// string converter class
class Converter
{
private:
public:
  // standard string types converters (Lua needs)
  // Windows 10 builds upon Unicode natively, thus using wide strings is wise.
  static std::wstring strConverter ( const std::string& );
  static std::string strConverter ( const std::wstring& );
};


#endif // !UTILITIES_H
