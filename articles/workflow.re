= ワークフロー

//comment{
ワークフローは、紙面以外の部分（この本を読む人は書きたいテーマはそろってるだろうから）を中心にRe :VIEWの外野の話（Re:VIEWでは埋められないところを伝えていくかんじかな。
ノンブルとかページ枚数の縛り、B5JとB5の違い、最低限入稿に必要な情報ってかんじ）前回は企画と頒布をフォローしたけど今回はページ数へらすからもうちょっとスコープ絞るのが適切そう。
TeXの話もレイアウト、スタイルみたいなのは飛ばしてしまいたいね。
//}

//comment{
初心者がMacTeXインストールせずに、すこしつかえるようにGrifletのインスタントモードみたいなのほしいね。Dockerファイルでいいんかな？これは思いつき。
//}

本書をここまで読んできて、Re:VIEWのいろはについては概ね諒解していただけたと思います。
この章では、実際に本を作るワークフローを紹介していきます。
TechBoosterがC81から練り上げてきた本を作るよくできた方法を解説します。

== 校正支援ツールprhを導入する

校正支援用ツールとしてprh@<fn>{prh}があります。
prhはproofread helperの略です。
proofreadで1単語だから略したらphだろ！とか言ってはいけません。

#@# prh:disable
prhは単純にルールに従い、文字列を置き換えるだけのものです。
たとえば、「例えば」と書いた時に「たとえば」に置き換えるよう促してくれます。
これを漢字をひらがなに「開く」操作といいます。

一般的な表現を用いることで文章を読みやすくする効果があります。
文章の質を高めるためにはこのような書き換え処理をたくさんこなす必要があるわけです。

TechBoosterでは原稿をみんなが書き終えた後、羊（@mhidaka、サークル主催）くんが全員の原稿に手を加え、開く処理をいれるなどの作業を行っていました。
そこでprhを作成し羊の作業から著者個人個人の作業に落とした@<fn>{crazy-mhidaka}わけです。
面白いもので、規則をルール化し記述できるようにしたことで著者の中での校正に対する関心も高まってきました。

肝心の使い方についてです。現状あまりコマンドラインツールとしての使い勝手はよくありません。
language-reviewにはprhが組み込まれているため、language-review経由で使うのが楽でしょう。

まずは、TechBoosterの設定ファイル（@<href>{https://github.com/vvakame/prh/blob/master/misc/techbooster.yml}）をダウンロードして試してみるのがよいでしょう。
ダウンロードしたファイルをprh.ymlという名前で.reファイルと同じディレクトリに置くとlanguage-reviewが校正用ルールとして参照して、Lintを表示します。

類似のツールとしてRedPen@<fn>{redpen}やtextlint@<fn>{textlint}などがあります。
さらなる文章の改善を目指すのであれば、これらのツールを利用してみるのもよいでしょう。

とはいえ、根本的に文章の目的と構造がしっかりしていないといくら枝葉末節を補ったところで意味はありません。

//footnote[prh][@<href>{https://github.com/vvakame/prh}]
//footnote[crazy-mhidaka][「羊の労力が減るぞ！」と僕が言ったら、彼は「じゃあもっと作れるな！」って言いました。あたまおかしい]
//footnote[redpen][@<href>{http://redpen.cc/}]
//footnote[textlint][@<href>{http://efcl.info/2015/09/10/introduce-textlint/}]

== GitHub Oriented Writing

略すとGoW… Gears of Warだ！
ハイ関係ないですね。

TechBoosterがどういう順番で執筆を行っているのか解説します。
1人だったり、2〜3人でやってたりするサークルだと少し大げさかもしれませんが参考になるでしょう。

技術書を書いてみたいという気持ちになったら友達を誘って共同で執筆を進めてみてください。
きっと締め切り前でも楽しいはずです。

プログラマであれば周知のとおり、一箇所にすべての情報が集まっているのが一番効率がよく、情報のありかがわからなくて困る事態にも陥りにくいです。

そのためネタ出しから目次の作成、執筆、レビューと手直し、編集までGitHub上で行っています（@<img>{workflow}）。

//image[workflow][よく分かる図解][scale=0.40]{
digraph G {
  planning [label="企画"];
  meeting [label="ネタ出し会議"];
  make_repo [label="GitHubにリポジトリ作成"];
  make_index [label="目次案作成"];
  writing [label="バリバリ執筆 & 修正"];
  pr_review [label="レビューをpull requestで送る"];
  merge [label="レビューのmerge"];
  freeze [label="執筆終了"];
  editing [label="編集作業"];
  submit [label="印刷所へ入稿"];

  planning   -> meeting;
  meeting    -> make_repo;
  make_repo  -> make_index;
  make_index -> writing;
  writing    -> pr_review;
  pr_review  -> merge;
  merge      -> writing;
  merge      -> freeze;
  freeze     -> editing;
  editing    -> submit;

  planning [shape=Mdiamond];
  submit [shape=Msquare];
}
//}

大まかな流れは次のとおりです。

 1. 企画会議に先立って1つのIssueに知りたいネタを書き込む
 2. オフラインで集合し、ネタの分配や争奪戦を行う
 3. 1冊＝1リポジトリでプライベートリポジトリを作成する
 4. 1ネタ=1Issue=1章という単位で目次案を書く
 5. ひたすら書く。masterに直pushでOK
 6. レビューを受ける
 7. レビュー指摘の修正作業を行う
 8. 編集者が校正＆調整作業に入る（直pushの禁止。変更はpull requestを送る）
 9. 印刷所への入稿作業。このフェーズからGitHubは利用されなくなる

いくつか補足します。

 * ひたすら書く。masterに直pushでOK

これは同一の.reファイルを複数人で編集する場合が少ないからです。
conflictが発生しないのであればpull requestにする必要性は薄いです。

WIP@<fn>{wip}として、書き終わるまでmasterにmergeしない方法もあります。
しかし、常にmasterで（もちろんビルドが通る状態であることは前提ですが）作業をすると、他の人の進捗もわかりやすく、本全体のページ数も掴みやすく、あまり執筆が進んでいない著者に圧力もかけやすくなります。

執筆というのは著者同士による圧力鍋バトルなので頑張りましょう。
早く書いてまだできていない人に圧力をかけねば自分が圧力をかけられる側に堕ちるのです。

話がそれました。

 * レビューを受ける

レビューは執筆の進捗に応じて２回程度行っています。

まずは構造が適切か、という視点でのレビューです。目次案、またはごく簡単に構成したスケルトン状態のものに関して、対象読者、筆者の視点、説明したい内容を十分カバーしている章立てか、など確認します。

この状態では主にIssueでのやり取りがメインです。

文章を書き上げた初校の段階で、もう少し詳細なレビューを行います。
@<chapref>{writing-book}でも少し触れましたが初校レビューはインラインに.reファイル中に書くケースが大半です。

これは、レビュー内容を取り込む時にWebサイトと見比べながら行うのが面倒であること、レビュワー同士で他にどういうコメントがあるかが把握できたほうが学びが多く、指摘の重複も少ないからです。

#@# prh:disable
またコメントを書いた後、直接masterにpushしない理由は筆者の修正とconflictが発生しやすいためです。
筆者は好きな時に好きなものを書き、気が向いた時にレビューの反映作業を行うのが精神衛生上良いでしょう。

また文章そのものへの指摘ではない、章の構造などより大きな視点で指摘する場合は、レビューのフェーズを問わずIssueのみを使って、やりとりを進めるケースもあります。

レビュー指摘をどう反映するかは著者の裁量に任せています。
もちろん、この後の行程では編集者が手を加えるのでヒエラルキーはレビュー＜執筆者＜編集者です。

これは納得のいく指摘のみ取り込んで欲しいからです。レビューをすべて取り込むことがよいことではありません。わかりやすい文章がすべてをカバーして書かれることは稀で、何を書かないかの選択が重要です。
編集時においても、著者の主張が適切に伝わるか、という観点で作業します。

 * 編集者が校正＆調整作業に入る（直pushの禁止。変更はpull requestを送る）

凍結後にmasterを変更する権限をもつのは編集者（TechBoosterでは羊）だけです。
執筆者がどうしても修正したい場合、pull requestを送りお伺いを立てます。
これを守らない場合、PDF化した時に改行の具合やら1行あたりの文字数やらを調整しているので編集者がぷっぷくぷー！になります。
可哀想なのでやめてあげましょう。

//footnote[wip][Work In Progress つまり書きかけ・作りかけの状態のこと]

== 執筆のための継続的インテグレーション - Griflet

エンジニアであればmasterビルドが壊れることを極端に怖がると思います。正しい。
そこでTechBoosterではCIサーバGriflet@<fn>{Griflet}を用意して常に最新のPDF出力が得られるような運用をしています。
これには多くのメリットがありますが中でも次のような点で執筆に貢献しています。

//footnote[Griflet][Griflet（グリフレット）はグリフ＋リーフレットの造語です。本に由来して名前を考えています。かっこいいとおもいません？僕は思います]

 * ビルド環境の準備が不要になる
 * Re:VIEWの文法エラーにすぐ気づける
 * レビュー対象物を特定できる

TechBoosterでは毎回40人程度が執筆に関わるので全員に特定のプラットフォームを強制することはできません。戦争が起きます。
CIとlanguage-reviewがあればローカルで文法チェックを行ってCIがビルドする、という形で最低限の執筆環境ができます。
またmasterが壊れている場合もすぐに気づけるため、常にクリーンな環境を維持できます。
エンジニアにとってビルドエラーの恐怖は説明するまでもありませんね。

レビュー期間も同様です。pushを停止させずにレビューしようと思うと、どこかにPDFがあることが望ましいわけです。
そこでCIでは入校時と同じスタイルを利用して常に入稿形式に準じたPDFを生成しています。
レビューを行うことで紙面レイアウトも含めてみることで素早く問題点に気づき、筆者にフィードバックを返すことができます。

実際の所、CIサーバの構築は手間がかかるのでTechBoosterで運用しているGrifletを公開したいのですが、
いまのところ負荷問題があり、それに至っていません。手軽に得られるビルド環境としてDockerfileを提供しています。

== 入稿のための最終出力を作る

印刷所へ最終的な印刷用データを渡すことを入稿と呼びます。部数に応じて適した印刷手法が異なります。

 * ～100部：オンデマンド印刷（版を作らない、小ロットに対応しているが単価は高い）
 * 100部～：オフセット印刷（版を作る。ロット数に応じて単価は安くなる）

一般的にオフセット印刷のほうが品質がよいですが、近年の印刷技術向上によりオンデマンド印刷でも十分な品質を得られるケースがあります。
とくに品質については印刷所の腕の見せ所といえます。紙の種類も多数あるため詳細を知りたい場合は印刷所に相談するとよいでしょう。
だれもが最初から熟練者ではありません。初めての人の相談にも快く応じてくれるでしょう。

入稿時の注意点は印刷所ごとに異なるので、ここから先はTechBoosterが過去に遭遇した事例をベースに説明します。
ちなみにTechBoosterでは日光企画さんを利用しています（ですので印刷所選びに不安があれば日光企画さんを使うとトラブルなく入稿できる可能性が高くなります）。

 * 表紙データ：表紙は本文と別の形式で入稿します
 * ページ数：紙の消費単位の関係で4の公倍数とするなど制約があります
 * ノンブル：全ページ通し番号が必要（番号は印刷ミスを防ぐためにつけます。同人誌以外ではみない制約です）
 * レイアウト：指がかかるのと本のノドがあるので余裕を持った小口（余白）を設定しましょう

#@# REVIEW vvakame ノド がわからんと思う

技術書であれば表紙がカラー、本文がモノクロであるケースがほとんどです。
ページ数は表紙も含んで数えますが基本的に４の倍数になるように揃えて入稿します（ならない場合は無理やり白いページを挟むこともあります）。

一番の悩みどころである印刷部数については売れて欲しい部数の８掛けぐらいちょうどいいでしょう。
鉄則は頒布する必要最低限だけ印刷して在庫を持たないことです。８掛けであれば予測を外しても大量には残らない数です。

同人誌においては新刊がもっとも売れます。残った在庫は既刊と呼ばれ、
新刊ほどの勢いはありません。おおよそ１０～２０％程度の販売速度になります。

サークルごとに部数の指標は違いますが、TechBoosterは１５００～２０００部を用意してます。
日光企画さんの場合はオンデマンド印刷、オフセット印刷どちらでもPDF入稿が可能です。
つまり本書で紹介しているRe:VIEWで出力したらそのまま入稿形式となり、完璧！というわけです。便利ですね。

== 紙面構成
本を作るうえで以外に苦労することのひとつに統一感というものがあります。文章が途中で変わっていないか、主張が変わっていないか、
タイトルと内容が変わったり、本の最後のほうで文体が変わったりするとやはりもったいないなぁ、という風になりますよね。

技術書として注意したらよいポイントをいくつか列挙していきます。粒度に差はありますが、重要だと思う項目からの紹介です。

=== 主張の統一
ひとつの文章にたくさんの主張を入れると読む人は混乱してしまいます。
読みやすさを考えてシンプルな構成を選びましょう。

 * 特定のコンテキストでは、ひとつの主張のみ扱う（章、節、項で主張の大小はある）
 * 初出の単語、概念は登場時点で説明する

たとえば章のはじめには導入（リード文）を用意してまとめを説明して読者と記事のマッチングをはかります。
書いているうちに節のタイトルが本文と異なる場合もありますので主張を意識して読みなおすことが大切です。
執筆中は初出の単語、概念など気づかない場合があります。読みなおすことで初出の単語の発見や、
前後の文章を入れ替えて主張を理解しやすくするなど、本の構成をよりよく変えられます。

=== 文中の文言と装飾を書籍にあわせる

#@# prh:disable
文章を書いていると上下を意識して「以上が」「以下のとおりです」など書きたくなりますが
組版の都合で上下が明らかでない場合があります。次のページにいってしまったりするわけですね。
それぞれ「前述のとおり」「次のとおりです」で置き換えるとよいでしょう@<fn>{lint}。
これらのために、@<chapref>{review-introduction}で述べたリストや図があるわけです。

//footnote[lint][前述のprhで技術書向きの校正を行えます。自分好みにカスタマイズするとよいでしょう]

また図、表、リストについては次のことを念頭に構成してください。

 * 図、表、リストの登場前に本文中からそれぞれ参照を行う（＠<img>、＠<list>など）
 * 登場前に内容を簡単に説明する一文を書く
 * 登場後に詳しい解説を書く

これらを守ることで何について述べているのかわかりやすくなり、読者が読んだときの唐突感や投げっぱなし感がなくなります。

文字の装飾については、これまでの同人活動を通じて使わないほうがよい、と感じてます。
装飾には、いわゆる太字、斜体などフォントの見た目を変えて読みやすくする意図があります。
しかし、利用基準を定めて守るほうが難しく、一貫性を保つのが作業量的にしんどい側面もあります。
大事な内容であれば、文章中で注意喚起したほうがよっぽどよいでしょう。
装飾が必要なときは編集担当者がチェックできる基準でのみ実施すべきです。
#@# REVIEW vvakame review-introduction.reで結構文字装飾使ってる…

たとえばリスト中の変数名ではコードとして装飾する、など明確な基準がある装飾であれば統一感がありますね。

=== 文体の統一
文体は著者の味となるため過剰な編集は好みませんが、それでも次のような表現は
編集段階で変更しています。

 * ネガティブな表現を利用しない
 * 体言止め、話し言葉は利用しない
 * ですます調と、である調を混在しない
 * 思いますなど感想にならないようにする
 * 文章中の記号は全角が基本（特にカッコ）
 * 文章中の英単語は前後にスペースを入れないで空きを詰める

ネガティブな表現は読者の心証を悪くします。期待して本を読んでいる読者に、
デメリットだけでなくメリットも感じてもらえるように表現をポジティブに改めます。
また技術書であれば平易な表現を心がけて読者の理解に努める方針のもと、
著者の文体を壊さない程度に調整します。
「～だと思います」という文章であれば「です」と編集したり、
「～することができます」という表現であれば簡潔に「～できます」と縮めています。

編集時には想定読者になりきって読みやすい文章を追及しましょう。
見た目に影響する話では、半角記号は基本的に英字に合わせてフォントが作られています。
文章中で表現として使う場合、記号は全角を利用してください。
メソッド名、プログラムのソースコードからの引用はその限りではありません。
とくに半角カッコはフォントの高さが全角文字と異なるため、沈んだ印象を受けます。
全角カッコを利用するようにしてください。
