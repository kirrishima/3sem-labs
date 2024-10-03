namespace OOP_Lab04
{
    public sealed class Button : ElemOfManage, IManagement
    {
        public void Show()
        {
            Console.WriteLine("Это кнопка");
        }

        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("Кнопка зажата");
            }
            else
            {
                Console.WriteLine("Кнопка не нажата");
            }

        }
    }
}
