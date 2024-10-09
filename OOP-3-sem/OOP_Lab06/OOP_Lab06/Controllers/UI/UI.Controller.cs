namespace OOP_Lab05.Controllers.UI
{
    public class UIController : UI
    {
        public List<Button> CountButtons()
        {
            List<Button> buttons = new List<Button>();
            foreach (var item in Elements)
            {
                if (item is Button)
                {
                    buttons.Add((Button)item);
                }
            }
            return buttons;
        }

        public int Count { get { return Elements.Count; } }

        public double CountArea()
        {
            double area = 0;
            foreach (var item in Elements)
            {
                if (item is Radiobutton)
                {
                    area += Math.PI * Math.Pow(((Radiobutton)item).Radius, 2);
                }
                if (item is Button)
                {
                    area += ((Button)item).Height * ((Button)item).Width;
                }
                if (item is Checktbox)
                {
                    area += Math.Pow(((Checktbox)item).Side, 2);
                }
            }
            return area;
        }
    }
}
