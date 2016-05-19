module.exports = function(grunt) {

    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
    
       less: {
            development: {
                options: {
                    paths: ['less'],
                    compress: false,
                    cleancss: true,
                    dumpLineNumbers: 'comments'
               },
                files: {
                    '../themes/theme1003/css/custom.css': '../themes/theme1003/less/custom.less'
                }
            }
        },
        
       ftp_push: {
           your_target: {
                options: {
		            authKey: "serverA",
    	           host: "alumnifu.ftp.ukraine.com.ua",
    	           dest: "/alumnifund.org/www",
    	           port: 21
              },
                files: [
                   {
                       expand: true,
                       cwd: '../modules/custom',
                        src: [
                       "**"
                        ],
                        dest: '/sites/all/modules/custom'
                    },
                    {
                       expand: true,
                       cwd: '../themes',
                        src: [
                       "**"
                        ],
                        dest: '/sites/all/themes'
                    }
               ]
            }
        },

        watch: {
            grunt: {
                files: ['Gruntfile.js']
            },

            less: {
                files: [
                    '../themes/theme1003/less/**/*.less'
                ],
                tasks: ['less', 'postcss']
            },
            
            ftppush: {
                files: [
                    '../modules/custom/**/*.*',
                    '../themes/**/*.*'
                ],
                tasks: ['ftp_push']
            },
          
        },
        
        postcss: {
           options: {
              processors: [
               require('autoprefixer')({browsers: ['last 2 versions', 'ie 10']}),
              ]
            },
            dist: {
              src: '../themes/theme1003/css/custom.css'
            }
        },     
    });

    // load npm modules
    grunt.loadNpmTasks('grunt-contrib-less');
    grunt.loadNpmTasks('grunt-contrib-watch');
    grunt.loadNpmTasks('grunt-ftp-push');
    grunt.loadNpmTasks('grunt-postcss');
    grunt.loadNpmTasks('grunt-contrib-copy');

    // register tasks
   // grunt.registerTask('default', [ 'ftp_push', 'watch']);
  //  grunt.registerTask('jenkins', [ 'ftp_push']);
   
    grunt.registerTask('default', ['less', 'postcss', 'ftp_push', 'watch']);
    grunt.registerTask('jenkins', ['less', 'postcss', 'ftp_push']);
};
