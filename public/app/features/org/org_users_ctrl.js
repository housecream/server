/*! grafana - v4.2.0 - 2017-03-22
 * Copyright (c) 2017 Torkel Ödegaard; Licensed Apache-2.0 */

System.register(["app/core/config","app/core/core_module"],function(a,b){"use strict";var c,d,e;b&&b.id;return{setters:[function(a){c=a},function(a){d=a}],execute:function(){e=function(){function a(a,b,d){this.$scope=a,this.$http=b,this.backendSrv=d,this.user={loginOrEmail:"",role:"Viewer"},this.get(),this.editor={index:0},this.showInviteUI=c.default.disableLoginForm===!1}return a.$inject=["$scope","$http","backendSrv"],a.prototype.get=function(){var a=this;this.backendSrv.get("/api/org/users").then(function(b){a.users=b}),this.backendSrv.get("/api/org/invites").then(function(b){a.pendingInvites=b})},a.prototype.updateOrgUser=function(a){this.backendSrv.patch("/api/org/users/"+a.userId,a)},a.prototype.removeUser=function(a){var b=this;this.$scope.appEvent("confirm-modal",{title:"Delete",text:"Are you sure you want to delete user "+a.login+"?",yesText:"Delete",icon:"fa-warning",onConfirm:function(){b.removeUserConfirmed(a)}})},a.prototype.removeUserConfirmed=function(a){this.backendSrv.delete("/api/org/users/"+a.userId).then(this.get.bind(this))},a.prototype.revokeInvite=function(a,b){b.stopPropagation(),this.backendSrv.patch("/api/org/invites/"+a.code+"/revoke").then(this.get.bind(this))},a.prototype.copyInviteToClipboard=function(a){a.stopPropagation()},a.prototype.openInviteModal=function(){var a=this.$scope.$new();a.invitesSent=this.get.bind(this);var b=this.showInviteUI?"public/app/features/org/partials/invite.html":"public/app/features/org/partials/add_user.html";this.$scope.appEvent("show-modal",{src:b,modalClass:"invite-modal",scope:a})},a}(),a("OrgUsersCtrl",e),d.default.controller("OrgUsersCtrl",e)}}});