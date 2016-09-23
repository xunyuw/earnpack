export default async (ctx, next) => {
  const title = 'koa2 title'

  return  ctx.body = {"hello": "xouyang"};
/*
  await ctx.render('index', {
    title
  })

  */
}
