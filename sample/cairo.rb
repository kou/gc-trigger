require "gc-trigger"
require "cairo"

def image_surface_finalizer(size)
  lambda do |id|
    GCTrigger.update_memory_usage(-size)
  end
end

def create_image_surface(width, height)
  size = 4 * width * height
  surface = Cairo::ImageSurface.new(:argb32, width, height)
  GCTrigger.update_memory_usage(size)
  ObjectSpace.define_finalizer(surface, image_surface_finalizer(size))
  surface
end

10000.times do |i|
  create_image_surface(6000, 6000)
  case File.readlines("/proc/self/status").grep(/\AVmRSS:/)[0]
  when /\AVmRSS:\s+(\d+)\s+kB/
    vm_rss = $1.to_i * 1024
  end
  puts [GC.count, i, vm_rss].join(",")
end
