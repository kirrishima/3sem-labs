using System;

namespace OOP_Lab04
{
    [Serializable]
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
                Console.WriteLine("Кнопка нажата");
            }
            else
            {
                Console.WriteLine("Кнопка не нажата");
            }

        }
    }
}
