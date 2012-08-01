a = 1
b = 1
c = "3"

if a == b
  puts "hello a == b" if a == b
  puts "hello c == b" if a == c

  if "test" == 3
    puts "it should not come here"
    puts "still shouldn't come here"
  end

  test1 = "epic"
  test2 = "epic"
  if test1 == test2
    puts "it should come here"
    puts "Well hello thar"
  end

  if test1 == test2
    puts "Yes"
  else
    puts "Nope"
  end
end

while a == 1
  puts "in while loop. Setting a to 2"
  a = 2
end

puts "test"
