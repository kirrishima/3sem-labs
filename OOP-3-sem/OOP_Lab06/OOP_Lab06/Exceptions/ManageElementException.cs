namespace OOP_Lab06.Exceptions
{
    internal class ManageElementException : OOP6Exception
    {
        public ManageElementException() : base("ManageElement exception occurred.") { }

        public ManageElementException(string message) : base(message) { }

        public ManageElementException(string message, Exception? innerException) : base(message, innerException) { }
    }
}
