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

require "rubygems"
require "bundler/gem_helper"
require "rake/extensiontask"

task :default => :compile

base_dir = File.join(File.dirname(__FILE__))

helper = Bundler::GemHelper.new(base_dir)
def helper.version_tag
  version
end

helper.install
spec = helper.gemspec

Rake::ExtensionTask.new("gc_trigger", spec) do |ext|
  ext.cross_platform = ["x86-mingw32", "x64-mingw32"]
  ext.cross_compile = true
end

def windows_gem_name(spec, architecture)
  "#{spec.name}-#{spec.version}-#{architecture}-mingw32.gem"
end

windows_architectures = [:x86, :x64]

namespace :build do
  namespace :windows do
    ruby_versions = "2.2.2:2.3.0"

    windows_architectures.each do |architecture|
      desc "Build gem for Windows #{architecture}"
      task architecture do
        build_dir = "tmp/windows"
        rm_rf build_dir
        mkdir_p build_dir

        commands = [
          ["git", "clone", "file://#{Dir.pwd}/.git", build_dir],
          ["cd", build_dir],
          ["bundle"],
          ["rake", "cross", "native", "gem", "RUBY_CC_VERSION=#{ruby_versions}"],
        ]
        raw_commands = commands.collect do |command|
          Shellwords.join(command)
        end
        raw_command_line = raw_commands.join(" && ")

        require "rake_compiler_dock"
        RakeCompilerDock.sh(raw_command_line)

        cp("#{build_dir}/pkg/#{windows_gem_name(spec, architecture)}",
           "pkg/")
      end
    end
  end

  desc "Build gems for Windows"
  build_tasks = windows_architectures.collect do |architecture|
    "windows:#{architecture}"
  end
  task :windows => build_tasks
end

namespace :release do
  desc "Push gems for Windows to RubyGems.org"
  task :windows do
    windows_architectures.each do |architecture|
      ruby("-S", "gem", "push", "pkg/#{windows_gem_name(spec, architecture)}")
    end
  end
end
