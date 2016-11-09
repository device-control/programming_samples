# coding: utf-8
if ARGV.length != 2 then
    puts "usage: bin2txt <in file> <out file>"
    exit
end
out = Array.new
binary = File.binread(ARGV[0])
index = 0
binary.unpack("C*").each do |c|
  out << "0x%02x,"%c
  index += 1
  if( (index % 16) == 0 )
    out << "\n"
  end
end
File.write(ARGV[1], out.join())
puts "complete."
