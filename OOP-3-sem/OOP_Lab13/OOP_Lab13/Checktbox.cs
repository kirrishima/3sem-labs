using System;

namespace OOP_Lab04
{
    [Serializable]
    public sealed class Checktbox : ElemOfManage, IManagement
    {
        public void Show()
        {
            Console.WriteLine("Это ChecktBox ");
        }

        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("ChecktBox отмечен");
            }
            else
            {
                Console.WriteLine("ChecktBox не отмечен");
            }

        }
    }
}
