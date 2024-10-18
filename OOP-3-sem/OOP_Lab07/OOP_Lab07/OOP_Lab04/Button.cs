namespace OOP_Lab04
{
    public sealed class Button : ElemOfManage, IManagement
    {
        public void Show()
        {
            Console.WriteLine($"Button [{PointX}, {PointY}]");
        }

        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("кнопка нажата");
            }
            else
            {
                Console.WriteLine("кнопка не нажата");
            }
        }

        public override void Resize(double x, double y)
        {
            PointX = x;
            PointY = y;
        }
    }
}

