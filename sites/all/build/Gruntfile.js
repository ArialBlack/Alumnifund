module.exports = function(grunt) {

    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
    
       // less: {
       //     development: {
       //         options: {
       //             paths: ['less'],
       //             compress: false,
        //            cleancss: true,
      //              dumpLineNumbers: 'comments'
       //         },
       //         files: {
       //             '../themes/vdnh_ui/css/style.css': '../themes/vdnh_ui/less/style.less'
      //          }
      //      }
     //   },
        
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
                    }
               ]
            }
        },

        watch: {
            grunt: {
                files: ['Gruntfile.js']
            },

          //  less: {
          //      files: [
          //          '../themes/vdnh_ui/less/**/*.less'
          //      ],
          //      tasks: ['less', 'postcss']
         //   },
            
            ftppush: {
                files: [
                    '../modules/custom/**/*.*',
                   // '!../themes/vdnh_ui/**/*.less'
                ],
                tasks: ['ftp_push']
            },
          
        },
        
       // postcss: {
      ///      options: {
       //       processors: [
        //        require('autoprefixer')({browsers: ['last 2 versions', 'ie 10']}),
       //       ]
      //      },
      //      dist: {
      //        src: '../themes/vdnh_ui/css/style.css'
      //      }
     //   },
        
     //   copy: {
    //        main: {
     //           files: [
     //               // includes files within path and its sub-directories
    //                {expand: true, src: ['../themes/vdnh_ui/**'], dest: 'd:/Open server/domains/vdnh-tech.dev/sites/all/themes/'},
    //            ],
    //        },
    //    },


        
    });

    // load npm modules
    grunt.loadNpmTasks('grunt-contrib-less');
    grunt.loadNpmTasks('grunt-contrib-watch');
    grunt.loadNpmTasks('grunt-ftp-push');
    grunt.loadNpmTasks('grunt-postcss');
    grunt.loadNpmTasks('grunt-contrib-copy');

    // register tasks
    grunt.registerTask('default', [ 'ftp_push', 'watch']);
    grunt.registerTask('jenkins', [ 'ftp_push']);
   
    //grunt.registerTask('default', ['less', 'postcss', 'copy:main', 'watch']);
   // grunt.registerTask('jenkins', ['less', 'postcss', 'copy:main']);
};
