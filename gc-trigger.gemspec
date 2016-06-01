# -*- ruby -*-
#
# Copyright (C) 2016  Kouhei Sutou <kou@clear-code.com>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library.  If not, see <http://www.gnu.org/licenses/>.

clean_white_space = lambda do |entry|
  entry.gsub(/(\A\n+|\n+\z)/, '') + "\n"
end

$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), "lib"))
require "gc-trigger/version"

Gem::Specification.new do |spec|
  spec.name = "gc-trigger"
  spec.version = GCTrigger::VERSION
  spec.homepage = "https://github.com/kou/gc-trigger"
  spec.authors = ["Kouhei Sutou"]
  spec.email = ["kou@clear-code.com"]
  readme = File.read("README.md", :encoding => "UTF-8")
  entries = readme.split(/^\#\#\s(.*)$/)
  description = clean_white_space.call(entries[entries.index("Description") + 1])
  spec.summary, spec.description, = description.split(/\n\n+/, 3)
  spec.license = "LGPL-3.0+"
  spec.files = ["README.md", "Rakefile", "Gemfile", "#{spec.name}.gemspec"]
  spec.files += Dir.glob("lib/**/*.rb")
  spec.files += Dir.glob("ext/**/*.{c,h,rb}")
  spec.files += Dir.glob("doc/text/*")
  spec.files += Dir.glob("sample/**/*.rb")
  spec.extensions += ["ext/gc_trigger/extconf.rb"]

  spec.add_development_dependency("bundler")
  spec.add_development_dependency("rake")
end
