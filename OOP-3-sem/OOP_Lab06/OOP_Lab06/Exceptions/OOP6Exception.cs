namespace OOP_Lab06.Exceptions
{
    internal class OOP6Exception : Exception
    {
        public OOP6Exception() : base("OOP6Exception occurred.") { }

        public OOP6Exception(string message) : base(message) { }

        public OOP6Exception(string message, Exception? innerException) : base(message, innerException) { }

        public static void ThrowIfNull(object argument, string? argumentName = null)
        {
            if (argument == null)
            {
                if (string.IsNullOrEmpty(argumentName))
                {
                    throw new OOP6Exception("Argument cannot be null.");
                }
                else
                {
                    throw new OOP6Exception($"Argument '{argumentName}' cannot be null.");
                }
            }
        }
    }
}
