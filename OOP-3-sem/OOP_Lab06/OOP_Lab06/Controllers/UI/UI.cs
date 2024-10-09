namespace OOP_Lab05.Controllers.UI
{
    public class UI
    {
        private readonly List<ManageElement> elements = [];

        public List<ManageElement> Elements
        {
            get { return elements; }
            set { elements.Clear(); elements.AddRange(value); }
        }

        public void Add(ManageElement element)
        {
            elements.Add(element);
        }

        public void Remove(ManageElement element)
        {
            elements.Remove(element);
        }

        public void PrintAll(List<ManageElement> manageElements)
        {
            Console.WriteLine($"[{string.Join(", ", manageElements)}]");
        }

        public void PrintAll()
        {
            PrintAll(elements);
        }
    }
}