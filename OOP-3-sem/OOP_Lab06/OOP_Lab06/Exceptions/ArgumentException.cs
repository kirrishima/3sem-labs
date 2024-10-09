namespace OOP_Lab06.Exceptions
{
    internal class ArgumentException : ManageElementException
    {
        public ArgumentException() : base("Argument excetpion.") { }

        public ArgumentException(string message) : base(message) { }

        public ArgumentException(string message, Exception? innerException) : base(message, innerException) { }
    }
}
