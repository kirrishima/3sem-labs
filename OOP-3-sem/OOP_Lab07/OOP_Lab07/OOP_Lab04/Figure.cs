namespace OOP_Lab04
{
    public abstract class Figure
    {
        public double pointX;
        public double pointY;

        public override string ToString()
        {
            return $"pointX: {pointX}, pointY: {pointY}";
        }

        public abstract void Show();
    }
}
