namespace OOP_Lab12
{
    public delegate void LoggingAction(string message);

    public class Logger
    {
        public enum LogMessageType
        {
            Info = 1,
            Action,
            Success,
            Warning,
            Error
        }

        protected readonly LoggingAction _loggingAction;

        protected static readonly Dictionary<LogMessageType, string> LogMessageTypeName = new()
        {
            { LogMessageType.Info,    "INFO" },
            { LogMessageType.Action,  "ACTION" },
            { LogMessageType.Success, "SUCCESS" },
            { LogMessageType.Warning, "WARNING" },
            { LogMessageType.Error,   "ERROR" }
        };

        public const string DefaultSeparator = " | ";
        protected readonly ConsoleColor _currentTimeColor = ConsoleColor.Green;
        protected static string _currentTime => DateTime.Now.ToString("MM-dd HH:mm:ss:ffff");
        protected static readonly int maxLogMessageLength = LogMessageTypeName.Values.Max(v => v.Length);

        public LoggingAction LoggingAction { get { return _loggingAction; } }

        public Logger(LoggingAction? loggingAction)
        {
            _loggingAction = loggingAction ?? throw new ArgumentNullException(nameof(loggingAction));
        }

        protected static string FormatLogMessage(string message)
        {
            int padding = (maxLogMessageLength - message.Length) / 2;
            return message.PadLeft(message.Length + padding).PadRight(maxLogMessageLength);
        }

        protected virtual void Log(LogMessageType type, string separator = " | ", params string[] messages)
        {
            _loggingAction(_currentTime);
            _loggingAction(separator);

            string formattedMessage = FormatLogMessage(LogMessageTypeName[type]);
            _loggingAction(formattedMessage);
            _loggingAction(separator);

            for (int i = 0; i < messages.Length; i++)
            {
                var message = messages[i];

                _loggingAction(message);

                if (i < messages.Length - 1)
                    _loggingAction(separator);
            }
            _loggingAction("\n");
        }

        public void PrintAllExeceptionsData(Exception? exception)
        {
            int level = 0;

            while (exception != null)
            {
                _loggingAction(new string('-', Console.WindowWidth));

                _loggingAction($"Error Nested Level: ");
                _loggingAction($"{level++}\n");

                _loggingAction($"Stack Trace: ");
                _loggingAction($"\n{exception.StackTrace}\n");

                _loggingAction($"Message: ");
                _loggingAction($"{exception.Message}\n");

                exception = exception.InnerException;
            }
        }

        public void Info(params string[] messages)
        {
            Log(type: LogMessageType.Info, separator: DefaultSeparator, messages: messages);
        }

        public void Success(params string[] messages)
        {
            Log(type: LogMessageType.Success, separator: DefaultSeparator, messages: messages);
        }

        public void Error(params string[] messages)
        {
            Log(type: LogMessageType.Error, separator: DefaultSeparator, messages: messages);
        }

        public void Warning(params string[] messages)
        {
            Log(type: LogMessageType.Warning, separator: DefaultSeparator, messages: messages);
        }
    }
}
