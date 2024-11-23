namespace OOP_Lab13.OOP_Lab04.Figures
{
    [Serializable]
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
