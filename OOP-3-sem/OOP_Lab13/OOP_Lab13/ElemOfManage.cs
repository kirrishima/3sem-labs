namespace OOP_Lab04
{
    [Serializable]
    public abstract class ElemOfManage
    {
        public bool Tap { get; set; }


        public override string ToString()
        {
            return $"Это элемент управления";
        }
    }
}
