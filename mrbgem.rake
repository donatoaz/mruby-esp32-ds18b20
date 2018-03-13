MRuby::Gem::Specification.new('mruby-esp32-ds18b20') do |spec|
  spec.license = 'MIT'
  spec.authors = 'AZEVEDO Donato'

  spec.cc.include_paths << "#{build.root}/src"
  spec.cc.include_paths << "#{build.root}/include"
end
