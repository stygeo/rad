class Epic
  def test(a)
  end

  def hello()
    Object.puts("Hello");
  end
end

a = Epic.new()
a.test()
a.hello()
Object.puts("hell", a.to_s())
Object.puts(a.to_s())
