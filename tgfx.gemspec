# fbgfx.gemspec

Gem::Specification.new do |s|
  s.name    = "tgfx"
  s.version = "0.1.0"
  s.summary = "Direct framebuffer manipulation graphics gem suitable for small framebuffer mapped tft-displays"
  s.author  = "Tony Ivanov"

  s.files = Dir.glob("ext/**/*.{c,rb}") +
            Dir.glob("lib/**/*.rb")

  s.extensions << "ext/tgfx/extconf.rb"

  s.add_development_dependency "rake-compiler"
  s.add_development_dependency "pry"
  s.add_development_dependency "chunky_png"
end
